cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "g++")

project(WebCpp  DESCRIPTION "An MVC framework for C++"
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)  # or 14, 17, 20, depending on your needs

add_executable(WebCpp src/main.cpp src/http/http.cpp)
