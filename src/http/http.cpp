#include "http.hpp"

namespace WebCpp
{
    HttpServer::HttpServer(int port, std::string_view addr) : hs_wsa{0},
                                                              hs_lstnSock{INVALID_SOCKET},
                                                              hs_connSock{INVALID_SOCKET},
                                                              hs_port{port},
                                                              hs_addr{addr},
                                                              hs_response{""},
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
            std::thread th(httpClientHandler, this);
            th.detach();
        }
        return 0;
    };

    int HttpServer::httpClientHandler(HttpServer* http)
    {
        HttpRequest req(http->hs_connSock);
        HttpResponse res(http->hs_connSock);
        URL url;
        Body bod;

        req.read();
        req.parseRequest(url, bod);

        std::string body{"<h1>Hello,World!</h1> \n <h2>How do you do?</h2> \n <h3>I am doing fine.</h3>"};
        res.buildBody(body);
        res.sendHeaders();
        res.flush();

        return 0;
    }

    // The http request structure function code
    HttpServer::HttpRequest::HttpRequest(SOCKET sen) : sender{sen} {}

    void HttpServer::HttpRequest::read() noexcept
    {
        this->req_raw.resize(4096);
        recv(this->sender, this->req_raw.data(), 4096, 0);
    }

    void HttpServer::HttpRequest::parseRequest(URL &u, Body &b) noexcept
    {
        std::istringstream st_rLine{this->req_raw.substr(0, this->req_raw.find('\r'))};
        std::istringstream st_headers{this->req_raw.substr(this->req_raw.find('\n'), (this->req_raw.find_last_of('\r') - 2))};
        std::string word{};

        std::getline(st_rLine, word, ' ');

        this->req_requestMethod = (word == "GET")     ? Methods::Get
                                  : (word == "PUT")   ? Methods::Put
                                  : (word == "POST")  ? Methods::Post
                                  : (word == "PATCH") ? Methods::Patch
                                                      : Methods::Delete;

        std::getline(st_rLine, word, ' ');
        u.setVar(word, word);
        word.clear();

        while (std::getline(st_headers, word))
            this->req_headers[word.substr(0, (word.find(':') + 1))] = word.substr((word.find(':') + 1), word.find(word.back()));

        for (auto &ref : this->req_headers)
            std::cout << ref.first << ref.second << std::endl;
    }

    // The Body Structure functions
    void HttpServer::Body::setRaw(byte data) noexcept
    {
        this->raw.push_back(data);
    }

    std::vector<byte> HttpServer::Body::getRaw() noexcept
    {
        return this->raw;
    }

    std::string HttpServer::Body::toString() noexcept
    {
        std::ostringstream strBody{};
        for (auto &ref : this->raw)
        {
            strBody << ref;
        }

        return strBody.str();
    }

    // The Http Response Structure function code
    HttpServer::HttpResponse::HttpResponse(SOCKET rec) : reciever{rec}
    {
    }

    void HttpServer::HttpResponse::buildBody(std::string &b)
    {
        this->res_body += b;
    }

    void HttpServer::HttpResponse::flush() noexcept
    {
        if (send(this->reciever, this->res_body.data(), this->res_body.size(), 0) != this->res_body.size())
            std::cout << "Flush failed" << std::endl;

        this->res_body.clear();
    }

    void HttpServer::HttpResponse::sendHeaders() noexcept
    {
        std::ostringstream head{};
        head << "HTTP/1.1";
        head << this->res_statusCode << " OK" << "\r\n";
        for (auto &ref : this->res_headers)
        {
            head << ref.first << ": " << ref.second;
        }
        head << "\r\n\r\n";

        if (!(send(this->reciever, head.str().data(), head.str().size(), 0) == head.str().size()))
            std::cout << "Header send failed" << std::endl;
    }

    // Http Server Destructor
    HttpServer::~HttpServer()
    {
        closesocket(this->hs_lstnSock);
        WSACleanup();
    };
}