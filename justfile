build:
    cd cpp && cmake .
    rm cpp/Makefile cpp/CMakeCache.txt cpp/cmake_install.cmake
    cargo tauri build
    cp cpp/cmake-build-debug/usuarios src-tauri/target/release/bundle/macos/Test.app/Contents/MacOS
    /Users/alejandro/CLionProjects/proyecto/src-tauri/target/release/bundle/macos/Test.app/Contents/MacOS/Test
