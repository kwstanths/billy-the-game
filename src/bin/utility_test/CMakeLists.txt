cmake_minimum_required (VERSION 3.0)

#Environment
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/SetupEnvironment.cmake)
SetupEnvironment()

set (NAME "utility_test")
project (${NAME})

include_directories(${PROJECT_ROOT}/src/lib/)
include_directories(${GLM_INCLUDE_DIRS})

file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)

add_executable(${NAME} 
	${${NAME}_HEADERS} ${${NAME}_SOURCES}
)

target_link_libraries(${NAME}
	debug_tools
	game_engine
)

