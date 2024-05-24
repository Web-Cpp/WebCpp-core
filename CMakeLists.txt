cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "g++")

project(WebCpp)

set(CMAKE_CXX_STANDARD 11)  # or 14, 17, 20, depending on your needs

add_executable(WebCpp main.cpp)
