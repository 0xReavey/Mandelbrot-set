cmake_minimum_required(VERSION 3.16)

project(AVA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(fractal)

target_sources(
	fractal
	PUBLIC
	include/mandelbrot.hpp
	
	src/main.cpp
	src/mandelbrot.cpp
)

target_compile_options(fractal PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(fractal PUBLIC include/)
target_link_libraries(fractal -lsfml-graphics -lsfml-window -lsfml-system)
