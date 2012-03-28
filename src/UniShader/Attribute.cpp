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

#include <UniShader/Attribute.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/OpenGL.h>
#include <UniShader/TypeResolver.h>
#include <UniShader/Buffer.h>

using UNISHADER_NAMESPACE;

Attribute::Attribute(ShaderProgram& program, std::string name):
m_program(program),
m_buffer(0),
m_name(name),
m_location(-1),
m_offset(0),
m_stride(0),
m_readingMode(ReadingMode::NONE),
m_normalize(false),
m_prepared(false){
	m_program.subscribeReceiver(signalPtr);
}

const std::string& Attribute::getClassName() const{
	static const std::string name("us::Attribute");
	return name;
}

Attribute::~Attribute(){
	m_program.unsubscribeReceiver(signalPtr);
}

void Attribute::connectBuffer(BufferBase::Ptr buffer, unsigned int offset, unsigned int stride){
	m_buffer = buffer;
	m_offset = offset;
	m_stride = stride;
	m_prepared = false;
	sendSignal(SignalID::CHANGED, this);
}

void Attribute::disconnectBuffer(){
	m_buffer = 0;
	m_prepared = false;
	sendSignal(SignalID::CHANGED, this);
}

void Attribute::normalize(bool norm){
	m_normalize = norm;
	m_prepared = false;
	sendSignal(SignalID::CHANGED, this);
}

unsigned int Attribute::getBufferOffset() const{
	return m_offset;
}

unsigned int Attribute::getBufferStride() const{
	return m_stride;
}

const GLSLType& Attribute::getGLSLType() const{
	//TODO: prepare automatically
	return m_type;
}

Attribute::ReadingMode Attribute::getReadingMode() const{
	return m_readingMode;
}

std::string Attribute::getName() const{
	return m_name;
}

void Attribute::setBufferOffset(unsigned int offset){
	m_offset = offset;
	sendSignal(SignalID::CHANGED, this);
}

void Attribute::setBufferStride(unsigned int stride){
	m_stride = stride;
	sendSignal(SignalID::CHANGED, this);
}
	
void Attribute::setReadingMode(ReadingMode readingMode){
	m_readingMode = readingMode;
	m_prepared = false;
	sendSignal(SignalID::CHANGED, this);
}

bool Attribute::prepare(){
	ensureGlewInit();
	clearGLErrors();

	if(m_program.getLinkStatus() != ShaderProgram::LinkStatus::SUCCESSFUL_LINK){
		std::cerr << "ERROR: Shader program is not linked" << std::endl;
		return FAILURE;
	}

	if(!m_prepared){

		m_location = glGetAttribLocation(m_program.getGlID(), m_name.c_str());
	
		printGLError();
		if(m_location == -1){
			std::cerr << "ERROR: Attribute " << m_name <<  " doesn't exist in program" << std::endl;
			return FAILURE;
		}

		GLsizei charSize = m_name.size()+1, size = 0, length = 0;
		GLenum type = 0;
		GLchar* name = new GLchar[charSize];
		name[charSize-1] = '\0';
		glGetActiveAttrib(m_program.getGlID(), m_location, charSize, &length, &size, &type, name); 

		delete name; name = 0;

		if(!TypeResolver::resolve(type, m_type))
			return FAILURE;

		m_prepared = true;
	}

	return SUCCESS;
}

void Attribute::apply(){
	ensureGlewInit();
	clearGLErrors();

	if(!m_buffer){
		std::cerr << "ERROR: Attribute " << m_name <<  " doesn't have buffer conected" << std::endl;
		return;
	}

	if(!prepare())
		return;

	//check for conflicts
	if(m_type.getDataType() == GLSLType::DataType::INT || m_type.getDataType() == GLSLType::DataType::UNSIGNED_INT){
		if(m_readingMode == ReadingMode::FLOAT || m_readingMode == ReadingMode::DOUBLE){
			std::cerr << "ERROR: Incompatible data type and data reading mode" << std::endl;
			return;
		}
	}
	else if(m_type.getDataType() == GLSLType::DataType::DOUBLE){
		if(m_readingMode != ReadingMode::DOUBLE){
			std::cerr << "ERROR: Incompatible data type and data reading mode" << std::endl;
			return;
		}
	}

	GLenum bufferDataType;
	size_t elemSize;
	switch(m_readingMode){
	case ReadingMode::CHAR:
		bufferDataType = GL_BYTE;
		elemSize = 1;
		break;
	case ReadingMode::UNSIGNED_CHAR:
		bufferDataType = GL_UNSIGNED_BYTE;
		elemSize = 1;
		break;
	case ReadingMode::SHORT:
		bufferDataType = GL_SHORT;
		elemSize = sizeof(short);
		break;
	case ReadingMode::UNSIGNED_SHORT:
		bufferDataType = GL_UNSIGNED_SHORT;
		elemSize = sizeof(unsigned short);
		break;
	case ReadingMode::INT:
		bufferDataType = GL_INT;
		elemSize = sizeof(int);
		break;
	case ReadingMode::UNSIGNED_INT:
		bufferDataType = GL_UNSIGNED_INT;
		elemSize = sizeof(unsigned int);
		break;
	case ReadingMode::FLOAT:
		bufferDataType = GL_FLOAT;
		elemSize = sizeof(float);
		break;
	case ReadingMode::DOUBLE:
		bufferDataType = GL_DOUBLE;
		elemSize = sizeof(double);
		break;
	default:
		std::cerr << "ERROR: Invalid data reading mode" << std::endl;
		return;
	}

	size_t stride = (m_stride + m_type.getColumnSize() * m_type.getColumnCount()) * elemSize;

	for(int i = 0; i < m_type.getColumnCount(); i++){
		glEnableVertexAttribArray(m_location+i);
		glBindBuffer(GL_ARRAY_BUFFER, m_buffer->getGlID());
		size_t offset = (m_offset + i*m_type.getColumnSize()) * elemSize;

		switch(m_type.getDataType()){
		case GLSLType::DataType::FLOAT:
			glVertexAttribPointer(m_location+i, m_type.getColumnSize(), bufferDataType, m_normalize, stride, (void*)offset);
			break;
		case GLSLType::DataType::DOUBLE:
			glVertexAttribLPointer(m_location+i, m_type.getColumnSize(), bufferDataType, stride, (void*)offset);
			break;
		case GLSLType::DataType::INT:
		case GLSLType::DataType::UNSIGNED_INT:
			glVertexAttribIPointer(m_location+i, m_type.getColumnSize(), bufferDataType, stride, (void*)offset);
			break;
		default:
			std::cerr << "ERROR: Invalid data type" << std::endl;
			return;
		}
			
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		printGLError();
	}
}

bool Attribute::handleSignal(unsigned int signalID, const ObjectBase* callerPtr){
	if(callerPtr->getClassName() == "us::ShaderProgram"){
		switch(signalID){
		case ShaderProgram::SignalID::RELINKED:
			m_prepared = false;
			return SUCCESS;
		}
	}
	return FAILURE;
}
