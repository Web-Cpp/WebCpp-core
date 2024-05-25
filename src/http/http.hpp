#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <string>
#include <string_view>

namespace WebCpp
{
    class HttpServer
    {
    private:
        WSADATA hs_wsa;
        SOCKET hs_lSock, hs_aSock;
        int hs_port;
        std::string hs_addr;
        std::string hs_request;
        std::string hs_response;
        SOCKADDR_IN hs_server;

    public:
        HttpServer(int p, std::string_view a);
        int initServer();
        ~HttpServer();

    private:
        int clientHandler();
    };
}

#endif