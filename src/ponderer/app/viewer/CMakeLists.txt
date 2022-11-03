add_executable(ponderer_app_viewer)
add_executable(ponderer::app:viewer ALIAS ponderer_app_viewer)

target_sources(ponderer_app_viewer
	PRIVATE
		main.cpp)

target_include_directories(ponderer_app_viewer
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ponderer_app_viewer
	PRIVATE
		ponderer::dep::all)
