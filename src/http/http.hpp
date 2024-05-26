#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include <string_view>

namespace WebCpp
{
    class HttpServer
    {
    private:
        WSADATA hs_wsa;
        SOCKET hs_lstnSock, hs_connSock;
        int hs_port;             // the port on which the server runs
        std::string hs_addr;     // the addr of the server
        std::string hs_request;  // the request from client
        std::string hs_response; // the response from client
        SOCKADDR_IN hs_server;   // the server struct consisted of communication types

    public:
        HttpServer(int port, std::string_view addr);
        int initServer();
        ~HttpServer();

    private:
        int buildResponse(std::string &msg);
        int clientHandler();
    };
}

#endif