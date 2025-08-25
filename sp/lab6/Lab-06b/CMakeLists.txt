cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER clang)

project(Lab-06b)

include_directories(includes)

file(GLOB SOURCES
    src/*.c
)

add_executable(Lab-06b ${SOURCES})