cmake_minimum_required(VERSION 3.9)
project(hw4)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw4 hw4.cc int16_set_bitvector.hh int16_set_trie.hh)

target_include_directories(hw4 PRIVATE bricks)
target_compile_options(hw4 PRIVATE -O2)


add_executable(hw4-test main.cc int16_set_bitvector.hh int16_set_trie.hh)
target_compile_options(hw4-test PRIVATE -Wall -Wextra -pedantic -fsanitize=address -g)
target_link_libraries(hw4-test PRIVATE asan)
