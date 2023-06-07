cmake_minimum_required(VERSION 3.24.1)
project(MeshTest CXX C)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(../vcglib ../vcglib/eigenlib ./)
link_directories(../triangle/build)

add_executable(
    MeshTest
    test.cpp
    Point.cpp
    Function.cpp
    Dissection.cpp
    Shape.cpp
    RecMesh.cpp
    # triangle.cpp
    TriMesh.cpp
    Convertcases.cpp
    ../vcglib/wrap/ply/plylib.cpp
)

target_link_libraries(MeshTest libtriangle.a)