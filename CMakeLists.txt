cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(tf 1.0)

find_package(argh)
add_executable(tf main.cpp)
target_link_libraries(tf argh)