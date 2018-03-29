cmake_minimum_required(VERSION 3.0)


# Environment
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/external/Modules/SetupEnvironment.cmake)
SetupEnvironment()

# Project
set(NAME "debug_tools")
project(${NAME})


# Sources
file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)



# Target
add_library(${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES})