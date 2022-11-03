find_package(glfw3 REQUIRED)

add_library(ponderer_dep_glfw INTERFACE)
add_library(ponderer::dep::glfw ALIAS ponderer_dep_glfw)

target_link_libraries(ponderer_dep_glfw
	INTERFACE
		glfw
		ponderer::common
		ponderer::dep::glad)
