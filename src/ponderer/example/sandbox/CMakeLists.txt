add_executable(ponderer_sandbox)

target_sources(ponderer_sandbox
	PRIVATE
		main.cpp)

target_include_directories(ponderer_sandbox
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ponderer_sandbox
	PRIVATE
		ponderer::dep::all)
