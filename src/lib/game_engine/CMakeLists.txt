cmake_minimum_required(VERSION 3.0)


# Environment
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/external/Modules/SetupEnvironment.cmake)
SetupEnvironment()


# Project
set(NAME "game_engine")
project(${NAME})
	

include_directories(${PROJECT_ROOT}/src/lib/)
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
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
file(GLOB ${NAME}_UTL_HEADERS utility/*.hpp)
file(GLOB ${NAME}_UTL_SOURCES utility/*.cpp)

# Target
add_library(
	${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES} 
	${${NAME}_GL_HEADERS} ${${NAME}_GL_SOURCES}
	${${NAME}_PHY_HEADERS} ${${NAME}_PHY_SOURCES}
	${${NAME}_MEM_HEADERS} ${${NAME}_MEM_SOURCES}
	${${NAME}_UTL_HEADERS} ${${NAME}_UTL_SOURCES}
	)

source_group("core\\Headers"	FILES	${${NAME}_HEADERS})
source_group("core\\Sources"	FILES	${${NAME}_SOURCES})
source_group("opengl\\Headers"	FILES	${${NAME}_GL_HEADERS})
source_group("opengl\\Sources"	FILES	${${NAME}_GL_SOURCES})
source_group("physics\\Headers"	FILES	${${NAME}_PHY_HEADERS})
source_group("physics\\Sources"	FILES	${${NAME}_PHY_SOURCES})
source_group("memory\\Headers"	FILES	${${NAME}_MEM_HEADERS})
source_group("memory\\Sources"	FILES	${${NAME}_MEM_SOURCES})
source_group("utility\\Headers"	FILES	${${NAME}_UTL_HEADERS})
source_group("utility\\Sources"	FILES	${${NAME}_UTL_SOURCES})

target_link_libraries(${NAME} ${FREETYPE_LIBRARY})
target_link_libraries(${NAME} debug_tools)
