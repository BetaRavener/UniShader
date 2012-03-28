/*
* UniShader - Interface for GPGPU and working with shader programs
* Copyright (c) 2011-2012 Ivan Sevcik - ivan-sevcik@hotmail.com
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

#include <UniShader/Buffer.h>
#include <UniShader/OpenGL.h>

#include <iostream>
#include <cstring>

using UNISHADER_NAMESPACE;

BufferBase::BufferBase():
m_byteSize(0),
m_frequencyMode(FrequencyMode::STATIC),
m_natureMode(NatureMode::DRAW),
m_bufferID(0){
	ensureGlewInit();
	clearGLErrors();

	glGenBuffers(1, &m_bufferID);
	printGLError();
}

const std::string& BufferBase::getClassName() const{
	static const std::string name("us::BufferBase");
	return name;
}

BufferBase::~BufferBase(){
	ensureGlewInit();
	clearGLErrors();

	glDeleteBuffers(1,&m_bufferID);
	printGLError();
}

bool BufferBase::clear(){
	ensureGlewInit();
	clearGLErrors();

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return !printGLError();
}

unsigned int BufferBase::getGlID() const{
	return m_bufferID;
}

BufferBase::FrequencyMode BufferBase::getFrequencyMode() const{
	return m_frequencyMode;
}

BufferBase::NatureMode BufferBase::getNatureMode() const{
	return m_natureMode;
}

void BufferBase::setFrequencyMode(FrequencyMode frequencyMode){
	m_frequencyMode = frequencyMode;
}
	
void BufferBase::setNatureMode(NatureMode natureMode){
	m_natureMode = natureMode;
}

bool BufferBase::mapBuffer(void** mappedPtr) const{
	ensureGlewInit();
	clearGLErrors();

	//map buffer from GPU memory to RAM
	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	*mappedPtr = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	if(printGLError()){
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return FAILURE;
	}
	return SUCCESS;
}

bool BufferBase::unmapBuffer() const{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return !printGLError();
}

bool BufferBase::setPlainData(const void* data, size_t size){
	ensureGlewInit();
	clearGLErrors();

	GLenum accessMode = 0;
	switch(m_frequencyMode){
	case FrequencyMode::STREAM:
		switch(m_natureMode){
		case NatureMode::DRAW:
			accessMode = GL_STREAM_DRAW;
			break;
		case NatureMode::READ:
			accessMode = GL_STREAM_READ;
			break;
		case NatureMode::COPY:
			accessMode = GL_STREAM_COPY;
			break;
		default:
			std::cerr << "ERROR: Invalid mode set for nature of access" << std::endl;
			return FAILURE;
		}
		break;
	case FrequencyMode::STATIC:
		switch(m_natureMode){
		case NatureMode:: DRAW:
			accessMode = GL_STATIC_DRAW;
			break;
		case NatureMode::READ:
			accessMode = GL_STATIC_READ;
			break;
		case NatureMode::COPY:
			accessMode = GL_STATIC_COPY;
			break;
		default:
			std::cerr << "ERROR: Invalid mode set for nature of access" << std::endl;
			return FAILURE;
		}
		break;
	case FrequencyMode::DYNAMIC:
		switch(m_natureMode){
		case NatureMode::DRAW:
			accessMode = GL_DYNAMIC_DRAW;
			break;
		case NatureMode::READ:
			accessMode = GL_DYNAMIC_READ;
			break;
		case NatureMode::COPY:
			accessMode = GL_DYNAMIC_COPY;
			break;
		default:
			std::cerr << "ERROR: Invalid mode set for nature of access" << std::endl;
			return FAILURE;
		}
		break;
	default:
		std::cerr << "ERROR: Invalid mode set for frequency of access" << std::endl;
		return FAILURE;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, data, accessMode);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(printGLError()){
		m_byteSize = 0;
		return FAILURE;
	}
	else{
		m_byteSize = size;
		return SUCCESS;
	}
}
