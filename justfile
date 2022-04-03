build tar="debug" post="nothing": compilar_cpp
    {{ if tar != "cpp" { "cp cpp/usuarios src-tauri/" } else { "" } }}
    {{ if tar != "cpp" { if tar == "debug" { "cargo tauri dev" } else { "cargo tauri build -d" } } else { "" } }}
    {{ if post == "run" { "./cpp/usuarios" } else if post == "leaks" { "leaks --atExit -- ./cpp/usuarios" } else { "" } }}
    

compilar_cpp:
    cd cpp && cmake .
    cd cpp && make
    rm -rf cpp/Makefile cpp/CMakeCache.txt cpp/cmake_install.cmake cpp/CMakeFiles cpp/liblibrerias.a