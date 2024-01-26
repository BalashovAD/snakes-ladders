cmake_minimum_required(VERSION 3.27)
project(SnakesAndLadders)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
        "./src/*.cpp"
        "./src/*.h"
        "./src/*.hpp"
        "./src/*.cxx")

add_executable(SnakesAndLadders ${SOURCES})
