cmake_minimum_required(VERSION 3.24.1)
project(MeshTest CXX C)
set(CMAKE_BUILD_TYPE "Debug")
include_directories(../vcglib ../vcglib/eigenlib ./)

add_executable(
    MeshTest
    test.cpp
    Point.cpp
    Function.cpp
    Dissection.cpp
    Shape.cpp
    RecMesh.cpp
    Convertcases.cpp
    ../vcglib/wrap/ply/plylib.cpp
)