cmake_minimum_required(VERSION 3.15)

find_package(glm REQUIRED COMPONENTS glm)
find_package(imgui REQUIRED COMPONENTS imgui)
find_package(glfw3 REQUIRED)

add_library(ponderer_common)

target_sources(ponderer_common
		PRIVATE
		${CMAKE_BINARY_DIR}/imgui_impl_glfw.cpp
		${CMAKE_BINARY_DIR}/imgui_impl_opengl3.cpp)

target_include_directories(ponderer_common
		PUBLIC
		${CMAKE_BINARY_DIR})

target_link_libraries(ponderer_common
		PUBLIC
		glfw
		glm::glm
		imgui::imgui)
