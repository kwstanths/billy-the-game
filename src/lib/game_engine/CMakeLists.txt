cmake_minimum_required(VERSION 3.0)

#Setup environment
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/SetupEnvironment.cmake)
SetupEnvironment()

# Project
set(NAME "game_engine")
project(${NAME})
	
include_directories(${ENGINE_INCLUDE_DIRS})

# Sources
file(GLOB ${NAME}_CORE_HEADERS core/*.hpp)
file(GLOB ${NAME}_CORE_SOURCES core/*.cpp)
file(GLOB ${NAME}_PHY_HEADERS physics/*.hpp)
file(GLOB ${NAME}_PHY_SOURCES physics/*.cpp)
file(GLOB ${NAME}_MATH_HEADERS math/*.hpp)
file(GLOB ${NAME}_MATH_SOURCES math/*.cpp)
file(GLOB ${NAME}_MEM_HEADERS memory/*.hpp)
file(GLOB ${NAME}_MEM_SOURCES memory/*.cpp)
file(GLOB ${NAME}_UTL_HEADERS utility/*.hpp)
file(GLOB ${NAME}_UTL_SOURCES utility/*.cpp)
file(GLOB ${NAME}_GPH_HEADERS graphics/*.hpp)
file(GLOB ${NAME}_GPH_SOURCES graphics/*.cpp)
file(GLOB ${NAME}_GL_HEADERS graphics/opengl/*.hpp)
file(GLOB ${NAME}_GL_SOURCES graphics/opengl/*.cpp)

# Target
add_library(
	${NAME} ${${NAME}_HEADERS} ${${NAME}_SOURCES} 
	${${NAME}_CORE_HEADERS} ${${NAME}_CORE_SOURCES}
	${${NAME}_PHY_HEADERS} ${${NAME}_PHY_SOURCES}
	${${NAME}_MATH_HEADERS} ${${NAME}_MATH_SOURCES}
	${${NAME}_MEM_HEADERS} ${${NAME}_MEM_SOURCES}
	${${NAME}_UTL_HEADERS} ${${NAME}_UTL_SOURCES}
	${${NAME}_GPH_HEADERS} ${${NAME}_GPH_SOURCES}
	${${NAME}_GL_HEADERS} ${${NAME}_GL_SOURCES}
	)

source_group("core\\Headers"		FILES	${${NAME}_CORE_HEADERS})
source_group("core\\Sources"		FILES	${${NAME}_CORE_SOURCES})
source_group("physics\\Headers"		FILES	${${NAME}_PHY_HEADERS})
source_group("physics\\Sources"		FILES	${${NAME}_PHY_SOURCES})
source_group("math\\Headers"		FILES	${${NAME}_MATH_HEADERS})
source_group("math\\Sources"		FILES	${${NAME}_MATH_SOURCES})
source_group("memory\\Headers"		FILES	${${NAME}_MEM_HEADERS})
source_group("memory\\Sources"		FILES	${${NAME}_MEM_SOURCES})
source_group("utility\\Headers"		FILES	${${NAME}_UTL_HEADERS})
source_group("utility\\Sources"		FILES	${${NAME}_UTL_SOURCES})
source_group("graphics\\Headers"	FILES	${${NAME}_GPH_HEADERS})
source_group("graphics\\Sources"	FILES	${${NAME}_GPH_SOURCES})
source_group("opengl\\Headers" 		FILES 	${${NAME}_GL_HEADERS})
source_group("opengl\\Sources" 		FILES 	${${NAME}_GL_SOURCES})

target_link_libraries(${NAME} ${ENGINE_LIBS})