cmake_minimum_required(VERSION 3.15)
project(ponderer CXX)

find_package(assimp REQUIRED)
find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

add_subdirectory(example)
add_subdirectory(src)
