cmake_minimum_required(VERSION 3.25.1)

project(space_squad)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(CMAKE_BUILD_TYPE Debug)

file(GLOB SOURCES
	"src/main.c"
	)

add_executable(${PROJECT_NAME} ${SOURCES})

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
target_compile_options(${PROJECT_NAME} PRIVATE
-Wall
-Wextra
-Wpedantic
#-Werror
-Wconversion
)
endif()

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

add_subdirectory(external/RodeoEngine)

add_dependencies(${PROJECT_NAME} RodeoEngine)

target_include_directories(${PROJECT_NAME}
	PUBLIC external/RodeoEngine/include
	)

target_link_libraries(${PROJECT_NAME}
	RodeoEngine
	)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
	DEPENDS RodeoEngine
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/external/RodeoEngine/build_dir/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
	)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	set_target_properties(space_squad PROPERTIES LINK_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s ALLOW_MEMORY_GROWTH")

	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
	-s MAX_WEBGL_VERSION=2 \
	-s WASM=1 \
	-s NO_EXIT_RUNTIME=1 \
	-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1 \
	-s OFFSCREENCANVAS_SUPPORT=1 \
	-s ALLOW_MEMORY_GROWTH=1\
	--preload-file shaders")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
	-fno-exceptions \
	-fno-rtti \
	-s USE_SDL=2")
	#-s USE_SDL_IMAGE=2 \

endif ()

