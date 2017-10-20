cmake_minimum_required(VERSION 3.9)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw1 hw1.cc hash_set_linked_list.hh hash_set_linear_probing.hh)
target_compile_options(hw1 PUBLIC -O2 -g -Wall -Wextra)
