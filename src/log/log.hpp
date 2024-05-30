#pragma once
#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <iostream>
#include <sstream>

namespace WebCpp::Log
{
     template <typename... TArgs>
     void log(TArgs&&... args) noexcept;

     template <typename... TArgs>
     void error(TArgs&&... args) noexcept;

     template <typename... TArgs>
     void warn(TArgs&&... args) noexcept;

     template <typename TFirst, typename... TArgs>
     void printImpl(const TFirst& first, TArgs&& ... args) noexcept;

     template <typename TLast>
     void printImpl(const TLast& last) noexcept;
}

template <typename... TArgs>
void WebCpp::Log::log(TArgs&&... args) noexcept
{
     std::cout << "\033[32m[INFO] \033[37m";
     printImpl(std::forward<TArgs>(args)...);
     std::cout << "\033[0m\n" << std::flush;
}

template <typename... TArgs>
void WebCpp::Log::warn(TArgs&& ... args) noexcept
{
     std::cout << "\033[33m[WARN] \033[37m";
     printImpl(std::forward<TArgs>(args)...);
     std::cout << "\033[0m\n" << std::flush;
}

template <typename... TArgs>
void WebCpp::Log::error(TArgs&& ... args) noexcept
{
     std::cout << "\033[31m[ERROR] \033[37m";
     printImpl(std::forward<TArgs>(args)...);
     std::cout << "\033[0m\n" << std::flush;
}


template <typename TFirst, typename... TArgs>
void WebCpp::Log::printImpl(const TFirst& first, TArgs&& ... args) noexcept
{
     std::cout << first << " ";
     printImpl(std::forward<TArgs>(args)...);
}

template <typename TLast>
void WebCpp::Log::printImpl(const TLast& last) noexcept
{
     std::cout << last;
}

#endif