cmake_minimum_required(VERSION 3.26)
project(nagyHazi C)

set(CMAKE_C_STANDARD 23)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

add_executable(sodoku2 main.c
        kiiras.c
        jatek.c
        econio.c
        UI.c
        UI.h
        fajlkezeles.c
        fajlkezeles.h)
target_link_libraries(sodoku2 m)
