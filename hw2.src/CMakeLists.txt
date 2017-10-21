cmake_minimum_required(VERSION 3.9)
project(hw2)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw1 hash_set_linked_list.hh hash_set_linear_probing.hh hw1.cc)
#target_compile_options(hw1 PUBLIC -g -Wall -Wextra -fsanitize=address)
target_compile_options(hw1 PUBLIC -g -Wall -Wextra -O2)
#target_link_libraries(hw1 PUBLIC asan)
target_include_directories(hw1 PUBLIC bricks)
