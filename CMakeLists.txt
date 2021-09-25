# cmake_minimum_required(VERSION <specify CMake version here>)
project(ClionProjects)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++")

add_executable(csgo_cheat main.cpp Memory.cpp Hide.h vector.h offs.h)
