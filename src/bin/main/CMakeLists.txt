cmake_minimum_required (VERSION 3.0)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Select build directory" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Source code directory and build directory are the same. Change it" )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Build directory contains spaces. Change it" )
endif()

set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
set (NAME "billy")

project (${NAME})

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
	ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
	ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t")
	#SET(GUI_TYPE WIN32)
endif()

find_package(OpenGL REQUIRED)

include_directories(${PROJECT_ROOT}/src/lib/)
include_directories(${PROJECT_ROOT}/src/lib/external/glfw-3.1.2/include/GLFW/)
include_directories(${PROJECT_ROOT}/src/lib/external/glm-0.9.7.1/)
include_directories(${PROJECT_ROOT}/src/lib/external/glew-1.13.0/include/)

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

