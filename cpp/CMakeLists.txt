cmake_minimum_required(VERSION 3.21)
project(usuarios)
# project(admin)
# project(librerias)

set(CMAKE_CXX_STANDARD 14)

# add_library(librerias banco.cpp utils.cpp)

add_executable(usuarios usuarios.cpp banco.cpp utils.cpp)
# add_executable(admin admin.cpp)

# target_link_libraries(admin l ibrerias)
# target_link_libraries(usuarios librerias)