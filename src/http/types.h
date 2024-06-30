#pragma once
#include <boost/beast/http.hpp>
// #include <boost/beast/core.hpp>

typedef boost::beast::http::response<boost::beast::http::string_body> TResponseBody;
typedef boost::beast::http::request<boost::beast::http::string_body>  TRequestBody;
