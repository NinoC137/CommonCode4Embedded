cmake_minimum_required(VERSION 3.20)
project(FFT C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(FFT
        FFT.c
        FFT.h test.c)

target_link_libraries(FFT m)
