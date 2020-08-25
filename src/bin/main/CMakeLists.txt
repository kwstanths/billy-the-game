cmake_minimum_required (VERSION 3.0)

#Environment
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/SetupEnvironment.cmake)
SetupEnvironment()

set (NAME "billy")
project (${NAME})

include_directories(${ENGINE_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)
file(GLOB ${NAME}_MAP_HEADERS Maps/*.hpp)
file(GLOB ${NAME}_MAP_SOURCES Maps/*.cpp)
file(GLOB ${NAME}_SHADERS ${PROJECT_ROOT}/src/shaders/*.glsl)

add_executable(${NAME}
    ${${NAME}_HEADERS} 
    ${${NAME}_SOURCES}
    ${${NAME}_MAP_HEADERS} ${${NAME}_MAP_SOURCES}
    ${${NAME}_SHADERS}
)

source_group("Maps\\Headers"		FILES	${${NAME}_MAP_HEADERS})
source_group("Maps\\Sources"		FILES	${${NAME}_MAP_SOURCES})
source_group("shaders"		 	FILES	${${NAME}_SHADERS})

target_link_libraries(${NAME} ${ENGINE_LIBRARY})

