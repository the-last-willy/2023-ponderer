add_executable(ponderer_example_sandbox_log)

target_sources(ponderer_example_sandbox_log
	PRIVATE
		main.cpp)

target_link_libraries(ponderer_example_sandbox_log
	PRIVATE
		ponderer::dep::all)
