cmake_minimum_required(VERSION 3.20)
project(gtkSweep C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)


add_executable(gtkSweep src/main.c
        src/game.c
        include/game.h
        src/rng.c
        include/error.h
        include/rng.h
        src/error.c
        src/ui.c
        src/ui.c
        src/ui.c
        include/ui.h
)

target_include_directories(gtkSweep
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(gtkSweep
        PRIVATE
        PkgConfig::GTK4
)
