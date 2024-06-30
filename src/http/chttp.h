#pragma once
#include "http/types.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <string>

namespace he
{
namespace http
{

class CHttp
{
public:
  CHttp();
  ~CHttp();
  void listen(const std::string &address, unsigned short port);

private:
  void do_session(boost::asio::ip::tcp::socket socket);
  TResponseBody handle_request(TRequestBody &req);

  TResponseBody notFound(TRequestBody &req, const std::string &target);
  TResponseBody serverError(TRequestBody &req, const std::string &what);
  TResponseBody content(TRequestBody &req, const std::string &text);
};

}
}
