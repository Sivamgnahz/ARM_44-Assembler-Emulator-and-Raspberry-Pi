cmake_minimum_required(VERSION 3.12)
project(arm11_44 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(arm11_44
        src/assembler.c
        src/assembler.h
        src/assembler_types.h
        src/assembler_yifei.c
        src/emulate.c
        src/emulate.h
        src/emulate_struct.h
        src/pass.h)
