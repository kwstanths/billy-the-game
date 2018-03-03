cmake_minimum_required (VERSION 3.0)

#Environment
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/external/Modules/SetupEnvironment.cmake)
SetupEnvironment()

set (NAME "billy")
project (${NAME})


find_package(OpenGL REQUIRED)

include_directories(${PROJECT_ROOT}/src/lib/)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})


set(ALL_LIBS
	${OPENGL_LIBRARY}
	debug_tools
	game_engine
	glfw
	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
)

file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)
file(GLOB ${NAME}_SHADERS ${PROJECT_ROOT}/src/shaders/*.glsl)

add_executable(${NAME}
    ${${NAME}_HEADERS} 
    ${${NAME}_SOURCES}
    ${${NAME}_SHADERS}
)

target_link_libraries(${NAME}
	${ALL_LIBS}
)

