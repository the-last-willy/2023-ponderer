find_package(glad COMPONENTS glad)

add_library(ponderer_dep_glad INTERFACE)
add_library(ponderer::dep::glad ALIAS ponderer_dep_glad)

target_link_libraries(ponderer_dep_glad
	INTERFACE
		glad::glad)
