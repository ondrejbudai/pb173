cmake_minimum_required(VERSION 3.6)
project(proj1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2")

set(SOURCE_FILES main.cpp)
add_executable(proj1 ${SOURCE_FILES})
