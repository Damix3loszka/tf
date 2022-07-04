cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(tf)

find_package(argh)

add_executable(tf main.cpp)
target_link_libraries(tf argh)
install(TARGETS tf DESTINATION bin)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK") # required
INCLUDE(CPack)