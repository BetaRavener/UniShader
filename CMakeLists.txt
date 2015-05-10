cmake_minimum_required( VERSION 2.8 )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE )
endif()

project( UniShader )

### USER INPUT ###

set( BUILD_SHARED_LIBS true CACHE BOOL "Build dynamic library." )

if( MSVC )
	set( STATIC_STD_LIBS false CACHE BOOL "True to link the runtime library statically, false to link them dynamically." )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )

# Set some useful compile flags.
if( CMAKE_COMPILER_IS_GNUCXX )
	# Define usage of c0x
	ADD_DEFINITIONS("-std=c++0x -DGLEW_MX")

	if( NOT CMAKE_CXX_FLAGS )
		set( CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic" CACHE STRING "C++ compiler flags" FORCE )
		set( CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic" CACHE STRING "C compiler flags" FORCE )
	endif()

	if( NOT CMAKE_CXX_FLAGS_DEBUG )
		set( CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
		set( CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING "C++ compiler flags (debug)" FORCE )
	endif()
endif()

# Make windows happy.
if( WIN32 )
	set( BIN_EXTENSION ".exe" )
	set( SHARE_TARGET_DIR . )
	add_definitions( -DWIN32 )
else()
	set( SHARE_TARGET_DIR share/UniShader )
endif()

set( LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib" )

set( INC_DIR "${PROJECT_SOURCE_DIR}/include" )
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src" )

set(
	SOURCES
	
	${INC_DIR}/UniShader/Attribute.h
	${INC_DIR}/UniShader/Buffer.h
	${INC_DIR}/UniShader/Buffer.inl
	${INC_DIR}/UniShader/Config.h
	${INC_DIR}/UniShader/GLSLType.h
	${INC_DIR}/UniShader/InternalBuffer.h
	${INC_DIR}/UniShader/ObjectBase.h
	${INC_DIR}/UniShader/OpenGL.h
	${INC_DIR}/UniShader/PrimitiveType.h
	${INC_DIR}/UniShader/SafePtr.h
	${INC_DIR}/UniShader/SafePtr.inl
	${INC_DIR}/UniShader/ShaderInput.h
	${INC_DIR}/UniShader/ShaderObject.h
	${INC_DIR}/UniShader/ShaderOutput.h
	${INC_DIR}/UniShader/ShaderOutput.inl
	${INC_DIR}/UniShader/ShaderProgram.h
	${INC_DIR}/UniShader/Signal.h
        ${INC_DIR}/UniShader/Texture.h
	${INC_DIR}/UniShader/TextureBuffer.h
	${INC_DIR}/UniShader/TextureUnit.h
	${INC_DIR}/UniShader/TypeResolver.h
	${INC_DIR}/UniShader/Uniform.h
	${INC_DIR}/UniShader/UniShader.h
	${INC_DIR}/UniShader/Utility.h
	${INC_DIR}/UniShader/Varying.h
	${INC_DIR}/UniShader/Varying.inl

	${SRC_DIR}/UniShader/Attribute.cpp
	${SRC_DIR}/UniShader/Buffer.cpp
	${SRC_DIR}/UniShader/GLSLType.cpp
	${SRC_DIR}/UniShader/InternalBuffer.cpp
	${SRC_DIR}/UniShader/OpenGL.cpp
	${SRC_DIR}/UniShader/ShaderInput.cpp
	${SRC_DIR}/UniShader/ShaderObject.cpp
	${SRC_DIR}/UniShader/ShaderOutput.cpp
	${SRC_DIR}/UniShader/ShaderProgram.cpp
	${SRC_DIR}/UniShader/Signal.cpp
        ${SRC_DIR}/UniShader/Texture.cpp
	${SRC_DIR}/UniShader/TextureBuffer.cpp
	${SRC_DIR}/UniShader/TextureUnit.cpp
	${SRC_DIR}/UniShader/TypeResolver.cpp
	${SRC_DIR}/UniShader/Uniform.cpp
	${SRC_DIR}/UniShader/UniShader.cpp
	${SRC_DIR}/UniShader/Varying.cpp
)

include_directories( ${INC_DIR} )
include_directories( ${OPENGL_INCLUDE_DIR} )
include_directories( ${GLEW_INCLUDE_PATH} )

# Static runtime linkage for Visual Studio.
if( MSVC AND STATIC_STD_LIBS )
	foreach( flag CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO )
		if( ${flag} MATCHES "/MD" )
			string( REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}" )
		endif()
	endforeach()
endif()

# Add the library.
if( BUILD_SHARED_LIBS )
	add_library( unishader SHARED ${SOURCES} )

	set_target_properties( unishader PROPERTIES DEBUG_POSTFIX -d )

	if ( WIN32 AND CMAKE_COMPILER_IS_GNUCXX )
		set_target_properties( unishader PROPERTIES PREFIX "" )
		set_target_properties( unishader PROPERTIES IMPORT_SUFFIX ".a" )
	endif()
else()
	add_definitions( -DUNISHADER_STATIC )
	
	add_library( unishader ${SOURCES} )

	set_target_properties( unishader PROPERTIES DEBUG_POSTFIX -s-d )
	set_target_properties( unishader PROPERTIES RELEASE_POSTFIX -s )
endif()

# Tell the compiler to export when necessary.
set_target_properties( unishader PROPERTIES DEFINE_SYMBOL UNISHADER_EXPORTS )

target_link_libraries( unishader ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLEW_LIBRARY} )

### INSTALL TARGETS ###

install(
	TARGETS unishader
	RUNTIME DESTINATION bin COMPONENT bin
	LIBRARY DESTINATION lib COMPONENT bin
	ARCHIVE DESTINATION lib COMPONENT dev
)

install(
	DIRECTORY include
	DESTINATION .
)

install(
	FILES README LICENSE
	DESTINATION ${SHARE_TARGET_DIR}
)
