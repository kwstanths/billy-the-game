if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)


### EXTERNAL LIBRARIES ### 
add_subdirectory(external)

### GAME ENGINE ###
add_subdirectory(game_engine)

### DEBUG TOOLS ###
add_subdirectory(debug_tools)
