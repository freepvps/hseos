project(magic)
cmake_minimum_required(VERSION 3.10)

string(APPEND CMAKE_CXX_FLAGS " -O3")

add_executable(main main.c)
