cmake_minimum_required (VERSION 3.0)


set(NAME "billy_the_game")
PROJECT(${NAME})

add_subdirectory(src/bin/main/)
add_subdirectory(src/bin/memory_test)
add_subdirectory(src/bin/utility_test)
add_subdirectory(src/bin/threedscene/)
add_subdirectory(src/bin/textures_co_unpack/)
add_subdirectory(src/lib/)
