cmake_minimum_required(VERSION 3.6)
project(brainfuck)

set(CMAKE_CXX_STANDARD 17)

add_executable(brainfuck brainfuck.cc)

target_compile_options(brainfuck PRIVATE -Wall -Wextra -pedantic)
