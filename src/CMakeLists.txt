cmake_minimum_required(VERSION 3.16)
project(src C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(src
        lexer.c
        lexer.h
        parser.c
        parser.h)
