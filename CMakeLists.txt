cmake_minimum_required(VERSION 2.8)
project(lemon)

add_definitions("-D__LINUX__")
add_definitions("-g -O0")
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(examples)
add_subdirectory(src)



