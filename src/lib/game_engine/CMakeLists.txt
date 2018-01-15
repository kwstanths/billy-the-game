cmake_minimum_required(VERSION 3.0)


# Environment
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)


# Project
set(NAME "game_engine")
project(${NAME})

# Condifure external packages include files 
set(FREETYPE_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/freetype/include/)

# Configuration
if(MSVC)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	add_definitions(-D_USE_MATH_DEFINES)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t")

	#Confifure external packages libraries
	set(FREETYPE_LIBRARY ${PROJECT_ROOT}/src/lib/external/freetype/lib/win32/freetype271.lib)
endif()

# Check compiler for c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(${PROJECT_ROOT}/src/lib/)
include_directories(opengl)
include_directories(${PROJECT_ROOT}/src/lib/external/glfw-3.1.2/include/GLFW/)
include_directories(${PROJECT_ROOT}/src/lib/external/glm-0.9.7.1/)
include_directories(${PROJECT_ROOT}/src/lib/external/glew-1.13.0/include/)
include_directories(${FREETYPE_INCLUDE_DIRS})

# Sources
file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)
file(GLOB ${NAME}_GL_HEADERS opengl/*.hpp)
file(GLOB ${NAME}_GL_SOURCES opengl/*.cpp)
file(GLOB ${NAME}_PHY_HEADERS physics/*.hpp)
file(GLOB ${NAME}_PHY_SOURCES physics/*.cpp)
file(GLOB ${NAME}_MEM_HEADERS memory/*.hpp)
file(GLOB ${NAME}_MEM_SOURCES memory/*.cpp)

# Target
add_library(
	${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES} 
	${${NAME}_GL_HEADERS} ${${NAME}_GL_SOURCES}
	${${NAME}_PHY_HEADERS} ${${NAME}_PHY_SOURCES}
	${${NAME}_MEM_HEADERS} ${${NAME}_MEM_SOURCES}
	)

source_group("core\\Headers"	FILES	${${NAME}_HEADERS})
source_group("core\\Sources"	FILES	${${NAME}_SOURCES})
source_group("opengl\\Headers"	FILES	${${NAME}_GL_HEADERS})
source_group("opengl\\Sources"	FILES	${${NAME}_GL_SOURCES})
source_group("physics\\Headers"	FILES	${${NAME}_PHY_HEADERS})
source_group("physics\\Sources"	FILES	${${NAME}_PHY_SOURCES})
source_group("memory\\Headers"	FILES	${${NAME}_MEM_HEADERS})
source_group("memory\\Sources"	FILES	${${NAME}_MEM_SOURCES})

target_link_libraries(${NAME} ${FREETYPE_LIBRARY})