cmake_minimum_required(VERSION 3.24.1)
project(MeshTest CXX C)

include_directories(../vcglib ../vcglib/eigenlib)

add_executable(
    MeshTest
    test.cpp
    Function.cpp
)