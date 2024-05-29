#include <iostream>
#include "http/http.hpp"
#include <memory>

int main(void)
{
     auto server{std::make_unique<WebCpp::HttpServer>(WebCpp::HttpServer(9090, "127.0.0.1"))};
     server->initServer();

     return 0;
}