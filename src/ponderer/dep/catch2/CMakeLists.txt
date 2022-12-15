find_package(Catch2)

add_library(ponderer_dep_catch2 INTERFACE)
add_library(ponderer::dep::catch2 ALIAS ponderer_dep_catch2)

target_link_libraries(ponderer_dep_catch2
	INTERFACE
		Catch2::Catch2WithMain)
