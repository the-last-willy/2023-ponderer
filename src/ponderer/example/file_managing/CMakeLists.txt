add_executable(ponderer_example_file_managing)
add_executable(ponderer::example:file_managing ALIAS ponderer_example_file_managing)

target_sources(ponderer_example_file_managing
		PRIVATE
		main.cpp)

target_include_directories(ponderer_example_file_managing
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ponderer_example_file_managing
		PRIVATE
		ponderer::dep::all)
