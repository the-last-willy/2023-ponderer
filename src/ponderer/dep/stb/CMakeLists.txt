find_package(stb COMPONENTS stb)

add_library(ponderer_dep_stb)
add_library(ponderer::dep::stb ALIAS ponderer_dep_stb)

target_sources(ponderer_dep_stb
	PRIVATE
		all.cpp)

target_include_directories(ponderer_dep_stb
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ponderer_dep_stb
	PUBLIC
		stb::stb)
