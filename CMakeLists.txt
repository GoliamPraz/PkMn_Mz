cmake_minimum_required(VERSION 3.16)
project(labirint_wasm)

# Емскриптен конфиг
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
    add_executable(game main.cpp)
    
    target_link_libraries(game raylib)
    
    set_target_properties(game PROPERTIES LINK_FLAGS "
        -s WASM=1
        -s USE_GLFW=3
        -s USE_WEBGL2=1
        -s FULL_ES3=1
        -s ALLOW_MEMORY_GROWTH=1
        -s TOTAL_MEMORY=67108864
        --shell-file ${CMAKE_SOURCE_DIR}/shell.html
        -o game.html
    ")
else()
    add_executable(game main.cpp)
    target_link_libraries(game raylib GL m pthread dl rt X11)
endif()
