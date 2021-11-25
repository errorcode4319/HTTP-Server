cmake_minimum_required(VERSION 3.1)
project(http-server)

file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(http-server ${SRC})

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(COMPILE_OPT "-g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_OPT} -std=c++17 -pthread -Wno-write-strings")

LINK_DIRECTORIES(${CMAKE_BINARY_DIR})
message(STATUS "${CMAKE_BINARY_DIR}")
message(STATUS "${CMAKE_SOURCE_DIR}")

