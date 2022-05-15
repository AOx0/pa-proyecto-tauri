cmake_minimum_required(VERSION 3.21)
project(usuarios)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif (WIN32)

add_executable(usuarios usuarios.cpp banco.cpp utils.cpp com.cpp)
target_link_libraries(usuarios ${CMAKE_SOURCE_DIR}/lcy-lib-c/target/release/liblcy_lib_c.a)