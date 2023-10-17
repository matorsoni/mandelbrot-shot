cmake_minimum_required(VERSION 3.10)

project(mandelbrot-shot)

add_executable(
    zoom
    deps/glad-4.0-core/src/glad.c
    src/main.cpp
)

target_include_directories(
    zoom
    PRIVATE
        deps/glfw/include
        deps/glad-4.0-core/include
        deps/stb/
)

add_subdirectory(deps/glfw)

target_link_libraries(
    zoom
    glfw
)
