add_executable(ponderer_imgui_demo)

target_sources(ponderer_imgui_demo
		PRIVATE
		imgui_demo/main.cpp)

target_include_directories(ponderer_imgui_demo
		PRIVATE
		imgui_demo)

target_link_libraries(ponderer_imgui_demo
		PRIVATE
		ponderer_common)

########################################################################################################################

add_executable(ponderer_sandbox)

target_sources(ponderer_sandbox
		PRIVATE
		sandbox/main.cpp)

target_include_directories(ponderer_sandbox
		PRIVATE
		sandbox)

target_link_libraries(ponderer_sandbox
		PRIVATE
		ponderer_common)
