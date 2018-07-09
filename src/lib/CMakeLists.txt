if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()


### DEBUG TOOLS ###
add_subdirectory(debug_tools)

### EXTERNAL LIBRARIES ### 
add_subdirectory(external)

### GAME ENGINE ###
add_subdirectory(game_engine)
