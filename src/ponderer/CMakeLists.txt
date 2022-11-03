add_subdirectory(app)
add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(dep)
add_subdirectory(example)
add_subdirectory(test)

add_library(ponderer_common INTERFACE)
add_library(ponderer::common ALIAS ponderer_common)

target_include_directories(ponderer_common
	INTERFACE
		..)

if(MSVC)
	target_compile_options(ponderer_common
		INTERFACE
			/std:c++20)
endif()
