add_subdirectory(assimp)
add_subdirectory(catch2)
add_subdirectory(fmt)
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(stb)

add_library(ponderer_dep_all INTERFACE)
add_library(ponderer::dep::all ALIAS ponderer_dep_all)

target_link_libraries(ponderer_dep_all
	INTERFACE
		ponderer::dep::assimp
		ponderer::dep::catch2
		ponderer::dep::fmt
		ponderer::dep::glad
		ponderer::dep::glfw
		ponderer::dep::glm
		ponderer::dep::imgui
		ponderer::dep::stb)
