#include "http.hpp"

namespace WebCpp
{
    HttpServer::HttpServer(int p, std::string_view a) : hs_wsa{0},
                                                        hs_lSock{INVALID_SOCKET},
                                                        hs_aSock{INVALID_SOCKET},
                                                        hs_port{p},
                                                        hs_addr{a},
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

        this->hs_lSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (this->hs_lSock == INVALID_SOCKET)
        {
            std::cout << "[ERROR] Socket Initialisation Failed" << std::endl;
            return -1;
        }

        if (bind(this->hs_lSock, (SOCKADDR *)&this->hs_server, sizeof(this->hs_server)) == INVALID_SOCKET)
        {
            std::cout << "[ERROR] Socket Binding Failed" << std::endl;
            return -1;
        }

        if (listen(this->hs_lSock, SOMAXCONN) == INVALID_SOCKET)
        {
            std::cout << "[ERROR] Socket Listening Failed" << std::endl;
            return -1;
        }
        std::cout << "[LOG] SOCKET LISTENING" << std::endl;

        this->hs_aSock = accept(hs_lSock, NULL, NULL);
        std::cout << "[LOG] CLIENT CONNECTED" << std::endl;

        this->hs_response.resize(30720);
        this->hs_response = "HTTP/1.1 200 OK \
        Content-Type: text/html; charset=utf-8 \
        Connection: Closed";
        this->hs_request.resize(30720);

        recv(this->hs_aSock, const_cast<char *>(this->hs_request.c_str()), this->hs_request.size(), 0);
        std::cout << this->hs_request.c_str() << std::endl;
        // send(this->hs_aSock, MESSAGE, 30720, 0);

        return 0;
    };

    HttpServer::~HttpServer()
    {
        closesocket(this->hs_lSock);
        WSACleanup();
    }
}