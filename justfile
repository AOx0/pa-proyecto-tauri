build type="release" post="nothing": compilar_cpp
    {{ if type != "cpp" { "cp cpp/usuarios src-tauri/" } else { "" } }}
    {{ if type != "cpp" { "just tauri type" } else { "" } }}
    {{ if post == "run" { "./cpp/usuarios" } else if post == "leaks" { "leaks --atExit -- ./cpp/usuarios" } else { "" } }}
    

compilar_cpp:
    cd cpp && cmake .
    cd cpp && make
    rm -rf cpp/Makefile cpp/CMakeCache.txt cpp/cmake_install.cmake cpp/CMakeFiles cpp/liblibrerias.a


tauri type:
    {{ if type == "release" { "cargo tauri build" } else { "cargo tauri dev" } }}
