cmake_minimum_required (VERSION 3.0)

set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
set (NAME "memory_test")
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

include_directories(${PROJECT_ROOT}/src/lib/)

file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)

add_executable(${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES}
)

target_link_libraries(${NAME}
	debug_tools
	game_engine
	${PROJECT_ROOT}../debug_tools/lib/debug_tools.lib
)

