build type="release": compilar_cpp
    cp cpp/usuarios src-tauri/
    {{ if type == "release" { "cargo tauri build" } else { "cargo tauri dev" } }}

compilar_cpp:
    cd cpp && cmake .
    cd cpp && make
    rm -rf cpp/Makefile cpp/CMakeCache.txt cpp/cmake_install.cmake cpp/CMakeFiles cpp/liblibrerias.a
    
cp: compilar_cpp
    ./cpp/usuarios