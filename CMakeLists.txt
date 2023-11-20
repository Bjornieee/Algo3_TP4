cmake_minimum_required(VERSION 3.26)
project(Algo3_TP4)

set(CMAKE_CXX_STANDARD 14)

include_directories(Code)

add_executable(Algo3_TP4
        Code/Makefile
        Code/rng.c
        Code/rng.h
        Code/skiplist.c
        Code/skiplist.h
        Code/skiplisttest.c)
