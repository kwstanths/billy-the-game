macro(SetupEnvironment)

	set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
	
	# Check compiler for c++11
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
			message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
	
	set(GLFW_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/glfw-3.1.2/include/GLFW/) 
	set(GLM_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/glm-0.9.7.1/) 
	set(GLEW_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/glew-1.13.0/include/)
	
	### OPENGL ###
	add_definitions(
		-DTW_STATIC
		-DTW_NO_LIB_PRAGMA
		-DTW_NO_DIRECT3D
		-DGLEW_STATIC
	)
	find_package(OpenGL REQUIRED)
	
	if(MSVC)
		#Prepare the building environment for Windows Visual Studio
		ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
		ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
		ADD_DEFINITIONS(-D_USE_MATH_DEFINES)
		ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
		ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
		#SET(GUI_TYPE WIN32)
	
		#Freetype
		set(FREETYPE_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/freetype/Windows/include/)
		set(FREETYPE_LIBRARY ${PROJECT_ROOT}/src/lib/external/freetype/Windows/lib/win32/freetype271.lib)
		
		#Assimp TODO assimp library is used
		set(ASSIMP_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/assimp-4.1.0/Windows/include/)
		set(ASSIMP_LIBRARY ${PROJECT_ROOT}/src/lib/external/assimp-4.1.0/Windows/lib/Debug/assimp-vc140-mt.lib)
        
        #Cal3D
        set(CAL3D_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/cal3d/include/)
        set(CAL3D_LIBRARY ${PROJECT_ROOT}/src/lib/external/cal3d/lib/Win32/cal3d.lib)
        
		
	elseif(UNIX)
		#Prepare the building environment for Linux G++
	
		#Freetype
		set(FREETYPE_INCLUDE_DIRS ${PROJECT_ROOT}/src/lib/external/freetype/Linux/include/)
		set(FREETYPE_LIBRARY ${PROJECT_ROOT}/src/lib/external/freetype/Linux/lib/libfreetype.a)
	
		find_package(ZLIB REQUIRED)
		if (ZLIB_FOUND)
			set(FREETYPE_INCLUDE_DIRS ${FREETYPE_INCLUDE_DIRS};${ZLIB_INCLUDE_DIRS})
			set(FREETYPE_LIBRARY ${FREETYPE_LIBRARY};${ZLIB_LIBRARIES})
		endif()
	
		#Assimp
		MESSAGE(STATUS "FATAL ERROR, ASSIMP NOT IMPLEMENTED FOR LINUX")
		
	endif()
	
	set(ENGINE_INCLUDE_DIRS 
		${PROJECT_ROOT}/src/lib/
		${GLFW_INCLUDE_DIRS} 
		${GLM_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${FREETYPE_INCLUDE_DIRS}
		${ASSIMP_INCLUDE_DIRS}
        ${CAL3D_INCLUDE_DIRS}
	)
	
	set(ENGINE_LIBS
		debug_tools
		glfw
		GLEW_1130
		${ASSIMP_LIBRARY}
		${FREETYPE_LIBRARY}
        ${CAL3D_LIBRARY}
		${OPENGL_LIBRARY}
	)
	
	set(ENGINE_LIBRARY ${ENGINE_LIBS} game_engine)

endmacro()