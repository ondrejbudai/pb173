cmake_minimum_required(VERSION 3.9)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw2 hash_set_linked_list.hh hash_set_linear_probing.hh hw1.cc)
target_compile_options(hw2 PUBLIC -g -Wall -Wextra -O2)
target_include_directories(hw2 PUBLIC bricks)
