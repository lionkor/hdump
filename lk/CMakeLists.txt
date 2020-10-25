project(lk)

include_directories(.)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra --pedantic -std=c17")

add_subdirectory(argsparser)
