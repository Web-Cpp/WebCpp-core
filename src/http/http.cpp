#include "http.hpp"

namespace WebCpp
{
    HttpServer::HttpServer(int port, std::string_view addr) : hs_wsa{0},
                                                              hs_lstnSock{INVALID_SOCKET},
                                                              hs_connSock{INVALID_SOCKET},
                                                              hs_port{port},
                                                              hs_addr{addr},
                                                              hs_response{""},
                                                              hs_request{""},
                                                              hs_server{} {};

    int HttpServer::initServer()
    {
        int iResult{0};
        iResult = WSAStartup(MAKEWORD(2, 2), &this->hs_wsa);

        if (iResult != 0)
        {
            std::cout << WSAGetLastError() << std::endl;
            return -1;
        }

        this->hs_server.sin_port = htons(this->hs_port);
        this->hs_server.sin_addr.s_addr = inet_addr(this->hs_addr.c_str());
        this->hs_server.sin_family = AF_INET;

        this->hs_lstnSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (this->hs_lstnSock == INVALID_SOCKET)
        {
            std::cout << "[ERROR] Socket Initialisation Failed" << std::endl;
            return -1;
        }

        if (bind(this->hs_lstnSock, (SOCKADDR *)&this->hs_server, sizeof(this->hs_server)) == SOCKET_ERROR)
        {
            std::cout << "[ERROR] Socket Binding Failed" << std::endl;
            return -1;
        }

        if (listen(this->hs_lstnSock, SOMAXCONN) == SOCKET_ERROR)
        {
            std::cout << "[ERROR] Socket Listening Failed" << std::endl;
            return -1;
        }
        std::cout << "[LOG] SOCKET LISTENING" << std::endl;

        while ((this->hs_connSock = accept(hs_lstnSock, NULL, NULL)) != SOCKET_ERROR)
        {
            std::thread th(&clientHandler, this);
            th.detach();
        }
        return 0;
    };

    int HttpServer::clientHandler()
    {
        constexpr size_t currentSize = 4096;
        this->hs_request.resize(currentSize);

        std::istringstream request{};

        recv(this->hs_connSock, this->hs_request.data(), currentSize, 0);

        std::cout << this->hs_request << std::endl;

        std::string msg = "<h1>Hello,World!</h1> \n <h2>How do you do?</h2> \n <h3>I am doing fine.</h3>";
        buildResponse(msg);
        if (send(this->hs_connSock, this->hs_response.data(), this->hs_response.size(), 0) == this->hs_response.size())
        {
            std::cout << this->hs_response << std::endl;
        }

        return 0;
    }

    int HttpServer::buildResponse(std::string &msg)
    {
        std::ostringstream response{};
        response << "HTTP/1.1 200 OK\r\n";
        response << "Content-Type: text/html\r\n";
        response << "Content-Length: " << msg.length() << "\r\n";
        response << "\r\n";
        response << msg << "\r\n\r\n";

        this->hs_response = response.str();
        return 0;
    }

    HttpServer::~HttpServer()
    {
        closesocket(this->hs_lstnSock);
        WSACleanup();
    };
}