cmake_minimum_required(VERSION 3.29)
project(hybrid_cryptosystem C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(hybrid_cryptosystem src/main.c
        include/rotor.h
        src/rotor.c
        include/des.h
        src/des.c)
