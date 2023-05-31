cmake_minimum_required(VERSION 3.25.1)

project(demo
	LANGUAGES C
	)

add_executable(${PROJECT_NAME}
	"src/main.c"
	"src/input.c"
	"src/player.c"
	"src/bullet.c"
	"src/enemies.c"
	"src/sprite.c"
	"src/wall.c"
	"src/debug.c"
	"src/init.c"
	"src/menu.c"
	)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# pull all git submodules recursively
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	# Update submodules as needed
	option(GIT_SUBMODULE "Check submodules during build" ON)
	if(GIT_SUBMODULE)
		message(STATUS "Submodule update")
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
		endif()
	endif()
endif()

add_subdirectory(external/RodeoKit)
add_dependencies(${PROJECT_NAME} RodeoKit)
target_include_directories(${PROJECT_NAME} PUBLIC
	external/RodeoKit/include
	src
	external/RodeoKit/external/cglm/include
	)
target_link_libraries(${PROJECT_NAME} PUBLIC RodeoKit)
target_link_libraries(${PROJECT_NAME} PUBLIC m)

if(DEFINED EMSCRIPTEN)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")

	# bundle in assets and shaders
	set_target_properties(${PROJECT_NAME}
		PROPERTIES
		LINK_FLAGS
		"--preload-file ${PROJECT_SOURCE_DIR}/external/RodeoKit/build_dir/shaders@shaders --preload-file ${PROJECT_SOURCE_DIR}/assets@assets --use-preload-plugins"
		)
endif()

if (NOT DEFINED EMSCRIPTEN)
	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		#-Werror
		-Wconversion
		)

	# copy over shaders to build directory
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		DEPENDS RodeoKit
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/external/RodeoKit/build_dir/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
		)

	# copy over assets to build directory
	add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
		)
endif()

