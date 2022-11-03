find_package(glm REQUIRED COMPONENTS glm)

add_library(ponderer_dep_glm INTERFACE)
add_library(ponderer::dep::glm ALIAS ponderer_dep_glm)

target_link_libraries(ponderer_dep_glm
	INTERFACE
		glm::glm
		ponderer::common)
