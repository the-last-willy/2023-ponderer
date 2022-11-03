add_executable(ponderer_example_imgui_demo)
add_executable(ponderer::example::imgui_demo ALIAS ponderer_example_imgui_demo)

target_sources(ponderer_example_imgui_demo
	PRIVATE
		main.cpp)

target_include_directories(ponderer_example_imgui_demo
	PRIVATE
		.)

target_link_libraries(ponderer_example_imgui_demo
	PRIVATE
		ponderer::dep::glfw
		ponderer::dep::imgui)
