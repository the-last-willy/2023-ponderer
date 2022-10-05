cmake_minimum_required(VERSION 3.15)
project(imgui_project CXX)

find_package(imgui REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} glfw imgui::imgui)
