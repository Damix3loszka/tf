cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 17)
project(tf)

find_package(argh)
add_executable(tf main.cpp)
target_link_libraries(tf argh)
set(template_path "/home/" $ENV{USER} "/projekty/tf/templates/")
file(WRITE build/tf.conf ${template_path})
file(MAKE_DIRECTORY build/template_folder)