cmake_minimum_required(VERSION 3.21)
project(proyecto)
project(usuarios)
project(admin)

set(CMAKE_CXX_STANDARD 14)

add_library(proyecto library.cpp)

add_executable(usuarios usuarios.cpp)
add_executable(admin admin.cpp)

target_link_libraries(admin proyecto)
target_link_libraries(usuarios proyecto)