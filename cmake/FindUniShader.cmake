#
# Try to find GLEW library and include path.
# Once done this will define
#
# UNISHADER_FOUND
# UNISHADER_INCLUDE_DIR
# UNISHADER_LIBRARY
# 

cmake_minimum_required( VERSION 2.8 )

set( UNISHADER_FOUND FALSE )

find_path(
	UNISHADER_INCLUDE_DIR
	UniShader/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		/usr
		/usr/local
		${UNISHADER_DIR}
)

find_library(
	UNISHADER_LIBRARY
	unishader
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${UNISHADER_DIR}
)

if( NOT UNISHADER_INCLUDE_DIR OR NOT UNISHADER_LIBRARY )
	message( FATAL_ERROR "UniShader not found." )
else()
	message( "UniShader found: ${UNISHADER_INCLUDE_DIR}" )
endif()