cmake_minimum_required(VERSION 3.25)
project(Filter C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Filter
        Filter.c
        Filter.h
        test.c)