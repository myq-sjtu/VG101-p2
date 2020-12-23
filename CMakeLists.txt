cmake_minimum_required(VERSION 2.7)
project(p2 C)

set(CMAKE_C_STANDARD 11)

add_executable(p2 main.c card.c card.h game.c game.h mode.c mode.h linkedlist.c linkedlist.h)