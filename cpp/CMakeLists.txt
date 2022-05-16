cmake_minimum_required(VERSION 3.21)
project(usuarios)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif (WIN32)

add_executable(usuarios usuarios.cpp banco.cpp utils.cpp com.cpp)

if (WIN32)
    target_link_libraries(usuarios ${CMAKE_SOURCE_DIR}/lcy-lib-c/target/release/lcy_lib_c.dll.lib)

    # copy DLL file into target dir
    add_custom_command(TARGET usuarios POST_BUILD              # Adds a post-build event to "main"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different      # which executes "cmake - E copy_if_different..."
            "${PROJECT_SOURCE_DIR}/lcy-lib-c/target/release/lcy_lib_c.dll" # <--this is in-file
            $<TARGET_FILE_DIR:usuarios>)                           # <--this is out-file path
    add_custom_command(TARGET usuarios POST_BUILD              # Adds a post-build event to "main"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different      # which executes "cmake - E copy_if_different..."
            "${PROJECT_SOURCE_DIR}/libwinpthread-1.dll" # <--this is in-file
            $<TARGET_FILE_DIR:usuarios>)                           # <--this is out-file path
else()
    target_link_libraries(usuarios ${CMAKE_SOURCE_DIR}/lcy-lib-c/target/release/liblcy_lib_c.a)
endif (WIN32)