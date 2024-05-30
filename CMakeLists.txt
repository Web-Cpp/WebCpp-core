cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "g++")

project(WebCpp  DESCRIPTION "An MVC framework for C++"
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(WebCpp src/main.cpp
        src/log/log.hpp
        src/log/log.cpp)
link_libraries(ws2_32)