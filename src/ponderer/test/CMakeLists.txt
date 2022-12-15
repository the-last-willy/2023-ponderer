add_executable(ponderer_test_unit)
add_executable(ponderer::test::unit ALIAS ponderer_test_unit)

target_sources(ponderer_test_unit
	PRIVATE
		unit/communicating.cpp)

target_link_libraries(ponderer_test_unit
	PRIVATE
		ponderer::common
		ponderer::dep::catch2)
