cmake_minimum_required(VERSION 3.16)
project(MLFQ C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(MLFQ
    src/mlfq.c
    src/mlfq.h
    src/process.c
    src/process.h
    src/util.c
    src/util.h)
