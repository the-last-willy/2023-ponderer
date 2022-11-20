find_package(fmt REQUIRED)

add_library(ponderer_dep_fmt INTERFACE)
add_library(ponderer::dep::fmt ALIAS ponderer_dep_fmt)

target_link_libraries(ponderer_dep_fmt
	INTERFACE
		fmt::fmt)
