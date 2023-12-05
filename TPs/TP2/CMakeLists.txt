cmake_minimum_required(VERSION 3.27)
project(S32 C)

set(CMAKE_C_STANDARD 11)

add_executable(S32 main.c
        server.c
        server.h)
