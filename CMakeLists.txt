cmake_minimum_required(VERSION 3.16)
project(lab3 C)

set(CMAKE_C_STANDARD 99)

include_directories(libs libs/headers headers)

add_executable(lab3 main.c
        libs/KGetLine.c
        libs/MyString.c
        headers/table_structs.h headers/table_creation.h headers/table_functions.h table_1_funcs.c table_2_funcs.c headers/commands.h table_creation.c headers/table_1_funcs.h headers/table_2_funcs.h libs/headers/functions_ptrs.h table_functions.c headers/spetial_operations.h spetial_operations.c)
