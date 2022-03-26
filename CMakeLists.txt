cmake_minimum_required(VERSION 3.5)

project(snabl)

set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-std=c++17 -stdlib=libc++ -fno-exceptions -Wall -Werror -g -O2)

file(GLOB_RECURSE src src/snabl/*.cpp)
add_executable(snabl ${src} src/main.cpp)
target_include_directories(snabl PUBLIC src/)
