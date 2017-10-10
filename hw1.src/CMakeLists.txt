cmake_minimum_required(VERSION 3.1)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw1 main.cc hash_set_linked_list.hh hash_set_linear_probing.hh)
target_compile_options(hw1 PUBLIC -O2 -g -Wall -Wextra)
