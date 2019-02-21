cmake_minimum_required (VERSION 3.0)

#Environment
set (PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../)
include(${PROJECT_ROOT}/src/lib/SetupEnvironment.cmake)
SetupEnvironment()

set (NAME "billy")
project (${NAME})

include_directories(${ENGINE_INCLUDE_DIRS})


file(GLOB ${NAME}_HEADERS *.hpp)
file(GLOB ${NAME}_SOURCES *.cpp)
file(GLOB ${NAME}_SHADERS ${PROJECT_ROOT}/src/shaders/*.glsl)

add_executable(${NAME}
    ${${NAME}_HEADERS} 
    ${${NAME}_SOURCES}
    ${${NAME}_SHADERS}
)

target_link_libraries(${NAME} ${ENGINE_LIBRARY})
target_compile_features(${NAME} PUBLIC cxx_std_17)

