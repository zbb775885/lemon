cmake_minimum_required(VERSION 2.8)
project(lemon)

#add_library(lemon SHARED include/lemon_abs_factory.hh)

add_executable(demo examples/main.cc)
target_include_directories(demo PRIVATE include)

#add_library(lemon SHARED)
#target_include_directories(lemon PRIVATE include)


