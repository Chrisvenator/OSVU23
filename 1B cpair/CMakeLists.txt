cmake_minimum_required(VERSION 3.26)
project(1B)

set(CMAKE_CXX_STANDARD 99)

include_directories(".")

add_executable(cpair "cpair.c" "cpair.h" "includes.h" "helperFunctions.c")
