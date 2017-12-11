cmake_minimum_required(VERSION 3.0)


# Environment
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)


# Project
set(NAME "game_engine")
project(${NAME})


# Configuration
if(MSVC)
	add_definitions(-D_WIN32_WINNT=0x0501)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	add_definitions(-D_USE_MATH_DEFINES)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t")
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
include_directories(${PROJECT_ROOT}/src/lib/external/glfw-3.1.2/include/GLFW/)
include_directories(${PROJECT_ROOT}/src/lib/external/glm-0.9.7.1/)
include_directories(${PROJECT_ROOT}/src/lib/external/glew-1.13.0/include/)

# Sources
file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)


# Target
add_library(${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES})
