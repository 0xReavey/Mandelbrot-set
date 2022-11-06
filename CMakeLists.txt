cmake_minimum_required(VERSION 3.16)

project(AVA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(fractorium)

target_sources(
	fractorium
	PUBLIC
	include/mandelbrot.hpp
	
	src/main.cpp
	src/mandelbrot.cpp
)

target_compile_options(fractorium PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(fractorium PUBLIC include/)
target_link_libraries(fractorium -lsfml-graphics -lsfml-window -lsfml-system)
