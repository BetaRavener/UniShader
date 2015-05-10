/*
* UniShader - Interface for GPGPU and working with shader programs
* Copyright (c) 2011-2013 Ivan Sevcik - ivan-sevcik@hotmail.com
*
* This software is provided 'as-is', without any express or
* implied warranty. In no event will the authors be held
* liable for any damages arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any
*    source distribution.
*/

#pragma once
#ifndef OPEN_GL_H
#define OPEN_GL_H

#include <GL/glew.h>
#include <iostream>
#include <string>

std::string& getGLExtensions();

inline void clearGLErrors(){
	while(glGetError() != GL_NO_ERROR){
	}
}

inline bool printGLError(std::string file, unsigned int line){
	unsigned int glErr;
	bool error = false;
	while( (glErr = glGetError()) != GL_NO_ERROR){
		std::cerr << "glError: " << glErr << " file: " << file << " @ " << line << "> " << gluErrorString(glErr) << std::endl; 
		error = true;
	}
	return error;
}

#ifdef GLEW_MX
GLEWContext* glewGetContext();
void setCurrentGLEWContext(GLEWContext* context);
#endif

#define printGLError() printGLError(__FILE__, __LINE__)

#endif
