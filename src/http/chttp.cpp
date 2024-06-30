#include "http/chttp.h"
#include <boost/beast/version.hpp>
#include <thread>

#include "st.h"
#include "chttp.h"



namespace he
{
namespace http
{

CHttp::CHttp()
{}

CHttp::~CHttp()
{}

void CHttp::listen(const std::string &address, unsigned short port)
{
  try
  {
    boost::asio::io_context ioc(std::max<int>(1, std::thread::hardware_concurrency()));
    boost::asio::ip::tcp::resolver resolver(ioc);

    auto const results = resolver.resolve(address, std::to_string(port));
    for (auto const& entry : results)
    {
      boost::asio::ip::tcp::acceptor acceptor{ioc, entry.endpoint()};
      for (;;)
      {
        boost::asio::ip::tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::thread(&CHttp::do_session, this, std::move(socket)).detach();
      }
    }
  }
  catch (const std::exception& e)
  {
    HE_ABORT("Error listening HTTP: " << e.what());
  }
}

void CHttp::do_session(boost::asio::ip::tcp::socket socket)
{
  try
  {
    boost::beast::flat_buffer buffer;
    while (true)
    {
      TRequestBody req;
      boost::beast::http::read(socket, buffer, req);
      auto res = handle_request(req);
      res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
      res.keep_alive(req.keep_alive());
      res.prepare_payload();
      boost::beast::http::write(socket, res);
      if (res.need_eof())
      {
        break;
      }
    }
  }
  catch (const boost::beast::system_error& se)
  {
    if (se.code() != boost::beast::errc::not_connected)
    {
      HE_LOG_ERR("Error: " << se.what());
    }
  }
  catch (const std::exception& e)
  {
    HE_LOG_ERR("Error: " << e.what());
  }

  boost::beast::error_code ec;
  socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
}

TResponseBody CHttp::notFound(TRequestBody &req, const std::string &target)
{
  TResponseBody res{boost::beast::http::status::not_found, req.version()};
  res.set(boost::beast::http::field::content_type, "text/html");
  res.body() = "The resource '" + target + "' was not found.";
  return res;
}

TResponseBody CHttp::serverError(TRequestBody &req, const std::string &what)
{
  TResponseBody res{boost::beast::http::status::internal_server_error, req.version()};
  res.set(boost::beast::http::field::content_type, "text/html");
  res.body() = "An error occurred: '" + what + "'";
  return res;
}

TResponseBody CHttp::content(TRequestBody &req, const std::string &text)
{
  TResponseBody res{boost::beast::http::status::ok, req.version()};
  res.set(boost::beast::http::field::content_type, "text/plain; version=0.0.4");
  res.set(boost::beast::http::field::content_length, std::to_string(text.size()));
  res.body() = text;
  HE_LOG_DBG("Content size: " << text.size());
  return res;
}

TResponseBody CHttp::handle_request(TRequestBody &req)
{
  if (req.method() != boost::beast::http::verb::get)
  {
    return serverError(req, "Invalid request method");
  }

  std::string urlpath = std::string(req.target());
  HE_LOG_VERBOSE("Incoming HTTP request: " <<  urlpath);

  if(urlpath == "/metrics") { return content(req, HE_ST.homed().metrics() + "\n"); }
  if(urlpath == "/list"   ) { return content(req, HE_ST.metrics().list());         }

  return notFound(req, urlpath);
}

}
}
