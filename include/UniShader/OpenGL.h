#pragma once
#ifndef OPEN_GL_H
#define OPEN_GL_H

#include <GL/glew.h>
#include <iostream>
#include <string>

std::string& getGLExtensions();
void ensureGlewInit();

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

#define printGLError() printGLError(__FILE__, __LINE__)

#endif