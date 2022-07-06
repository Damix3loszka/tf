cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(tf VERSION 1)
add_executable(tf main.cpp)
target_include_directories(tf PRIVATE include)
install(TARGETS tf DESTINATION /usr/local/bin)
install(TARGETS tf DESTINATION /bin)