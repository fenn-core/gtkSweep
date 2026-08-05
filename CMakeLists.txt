cmake_minimum_required(VERSION 3.60)
project(gtkSweep C)

set(CMAKE_C_STANDARD 11)

add_executable(gtkSweep src/main.c
        src/game.c
        include/game.h
        include/game.h
)

target_include_directories(gtkSweep
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
