cmake_minimum_required(VERSION 3.10)

project(mandelbrot-shot)

add_executable(
    zoom
    src/main.cpp
)

target_include_directories(
    zoom
    PRIVATE
        deps/glfw/include
        deps/glad-4.0-core/include
)
