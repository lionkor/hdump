cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=c17 -g")

add_subdirectory(lk)

project(hdump)

add_executable(hdump
    hdump.c
)

target_link_libraries(hdump lk_argsparser)
