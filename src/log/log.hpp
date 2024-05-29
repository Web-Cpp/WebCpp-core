#pragma once
#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <iostream>
#include <sstream>

namespace WebCpp
{
    class Log
    {
    private:
        enum struct logLevel : char
        {
            LogLevelError = 1,
            LogLevelWarn = 2,
            LogLevelLog = 3,
        };

        logLevel level;
        std::string msg;

    public:
        void setLevel(logLevel n) noexcept;
        void log(std::string_view l) noexcept;
        void error(std::string_view e) noexcept;
        void warn(std::string_view w) noexcept;

    private:
        void print();
    };
}

#endif