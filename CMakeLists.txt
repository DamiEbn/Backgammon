cmake_minimum_required(VERSION 4.1)
project(Backgammon C)

set(CMAKE_C_STANDARD 11)

add_executable(Backgammon backgammon.c)

target_compile_options(Backgammon PRIVATE -Wall -Werror)
