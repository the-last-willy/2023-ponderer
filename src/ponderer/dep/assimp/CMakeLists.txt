find_package(assimp COMPONENTS assimp)

add_library(ponderer_dep_assimp INTERFACE)
add_library(ponderer::dep::assimp ALIAS ponderer_dep_assimp)

target_link_libraries(ponderer_dep_assimp
	INTERFACE
		assimp::assimp
		ponderer::common)
