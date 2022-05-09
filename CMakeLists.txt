cmake_minimum_required(VERSION 3.22)
project(pa_proyecto_tauri)

set(CMAKE_CXX_STANDARD 14)

include_directories(cpp)

add_executable(pa_proyecto_tauri
        cpp/banco.cpp
        cpp/banco.h
        cpp/usuarios.cpp
        cpp/utils.cpp
        cpp/utils.h)
