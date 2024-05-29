#pragma once
#ifndef HTTP_HPP
#define HTTP_HPP

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <thread>

#include <vector>
#include <unordered_map>

#include <iostream>
#include <sstream>
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
        std::string hs_response; // the response from client
        SOCKADDR_IN hs_server;   // the server struct consisted of communication types

        // The HTTP request methods
        enum struct Methods : char
        {
            Get,
            Post,
            Put,
            Patch,
            Delete,
        };

        // the body of the request
        struct Body
        {
        private:
            std::vector<byte> raw;

        public:
            std::string toString() noexcept;
            std::vector<byte> getRaw() noexcept;
            void setRaw(byte data) noexcept;
        };

        // the target UTI of request and query
        struct URL
        {
        private:
            std::string path;
            std::string query;

        public:
            void setVar(std::string &p, std::string &q) noexcept;
            std::string toString();
        };

        // HttpRequest structure
        struct HttpRequest
        {
        private:
            std::string req_raw;                                      // the raw request from client
            Methods req_requestMethod;                                // the request method
            std::unordered_map<std::string, std::string> req_headers; // the headers of the request
            SOCKET sender;                                            // the request sender

        public:
            HttpRequest(SOCKET sen);
            std::string getHeaders() noexcept;
            void parseRequest(URL &u, Body &b) noexcept;
            void read() noexcept;
        };

        struct HttpResponse
        {
            HttpResponse(SOCKET rec);
            void sendHeaders() noexcept;
            void buildBody(std::string &b);
            void flush() noexcept;

        private:
            int res_statusCode;
            std::unordered_map<std::string, std::string> res_headers;
            std::string res_body;
            SOCKET reciever;
        };

    public:
        HttpServer(int port, std::string_view addr);
        int initServer();
        ~HttpServer();

    private:
        int buildHttpResponse(HttpResponse &res);
        int httpClientHandler();
        int parseHttpRequest(HttpRequest &req);
    };
}

#endif