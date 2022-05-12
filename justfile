build tar="debug" post="run": compilar_cpp
    {{ if tar != "cpp" { "cp cpp/usuarios src-tauri/" } else { "echo Skip" } }}
    {{ if tar != "cpp" { if tar == "debug" { "cd src-tauri && cargo run" } else { "cargo tauri build -d" } } else { "echo Skip" } }}
    {{ if tar == "cpp" { if post == "run" { "./cpp/usuarios" } else if post == "leaks" { "leaks --atExit -- ./cpp/usuarios" } else { "echo Skip" } } else { "echo Skip" } }}
    {{ if tar != "cpp" { if post == "run" { if tar == "debug" { "echo Skip" } else { "open ./src-tauri/target/debug/bundle/macos/SuperBanco.app" } } else { "echo Skip" } } else { "echo Skip" } }}

compilar_cpp:
    cd cpp && cmake .
    cd cpp && make
    rm -rf cpp/Makefile cpp/CMakeCache.txt cpp/cmake_install.cmake cpp/CMakeFiles cpp/liblibrerias.a