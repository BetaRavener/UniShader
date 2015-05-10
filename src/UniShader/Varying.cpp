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

#include <UniShader/Varying.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/ShaderOutput.h>
#include <UniShader/Buffer.h>
#include <UniShader/OpenGL.h>
#include <UniShader/TypeResolver.h>
#include <UniShader/InternalBuffer.h>
#include <string.h>

using UNISHADER_NAMESPACE;

Varying::Varying(ShaderProgram& program, ShaderOutput& output, std::string name):
m_program(program),
m_output(output),
m_name(name),
m_buffer(0),
m_unitSize(0),
m_prepared(false){
	m_program.subscribeReceiver(signalPtr);
	m_output.subscribeReceiver(signalPtr);
	if(!m_output.isInterleaved()){
		m_buffer = BufferBase::Ptr(std::dynamic_pointer_cast<BufferBase>(InternalBuffer::create()));
		m_buffer->setFrequencyMode(BufferBase::FrequencyMode::DYNAMIC);
		m_buffer->setNatureMode(BufferBase::NatureMode::COPY);
	}
}

const std::string& Varying::getClassName() const{
	static const std::string name("us::Varying");
	return name;
}

Varying::~Varying(){
	m_program.unsubscribeReceiver(signalPtr);
}

const GLSLType& Varying::getGLSLType() const{
	return m_glslType;
}

std::string Varying::getName() const{
	return m_name;
}

bool Varying::prepare(unsigned int index, unsigned int primitiveCount, size_t* unitSize){
	size_t bufSize = 0;

	if(m_program.getLinkStatus() != ShaderProgram::LinkStatus::SUCCESSFUL_LINK){
		std::cerr << "ERROR: Shader program is not linked" << std::endl;
		return FAILURE;
	}

	if(!m_prepared){
		GLsizei charSize = m_name.size()+1, size = 0, length = 0;
		GLenum type = 0;
		GLchar* name = new GLchar[charSize];
		name[charSize-1] = '\0';

		GLint varyingCount;
		glGetProgramiv(m_program.getGlID(), GL_TRANSFORM_FEEDBACK_VARYINGS, &varyingCount);
		for(int i = 0; i < varyingCount; i++){
			glGetTransformFeedbackVarying(m_program.getGlID(), index, charSize+1, &length, &size, &type, name);
			if(length == m_name.size()){
				//if names match break search
				if(memcmp(name, m_name.c_str(), length) == 0)
					break;
			}
		}

		delete[] name; name = 0;

		if(length == 0){
			std::cerr << "ERROR: Varying doesn't exist in program" << std::endl;
			return FAILURE;
		}

		if(!TypeResolver::resolve(type, m_glslType))
			return FAILURE;

		//DRIVER ISSUE
		if(m_glslType.getColumnCount() > 1 && !m_output.isInterleaved()){
			std::cerr << "ERROR: Transform feedback in separate attribs mode can't be used whit matrices (break to vectors or use interleaved mode)" << std::endl;
			return FAILURE;
		}

		m_unitSize = m_glslType.getColumnCount() * m_glslType.getColumnSize();
		switch(m_glslType.getDataType()){
		case GLSLType::DataType::FLOAT:
			m_unitSize *= sizeof(GLfloat);
			break;
		case GLSLType::DataType::INT:
			m_unitSize *= sizeof(GLint);
			break;
		case GLSLType::DataType::UNSIGNED_INT:
			m_unitSize *= sizeof(GLuint);
			break;
		default:
			std::cerr << "ERROR: Invalid data type" << std::endl;
			return FAILURE;
		}

		m_prepared = true;
	}

	if(unitSize)
		*unitSize = m_unitSize;

	if(!m_output.isInterleaved()){
		bufSize = m_unitSize * primitiveCount;
		if(bufSize != std::static_pointer_cast<InternalBuffer>(m_buffer)->getSize() && bufSize != 0)
			std::static_pointer_cast<InternalBuffer>(m_buffer)->resize(bufSize);
	}

	return SUCCESS;
}

bool Varying::handleSignal(unsigned int signalID, const ObjectBase* callerPtr){
	if(callerPtr->getClassName() == "us::ShaderProgram"){
		switch(signalID){
		case ShaderProgram::SignalID::RELINKED:
			m_prepared = false;
			return SUCCESS;
		}
	}
	if(callerPtr->getClassName() == "us::ShaderOutput"){
		switch(signalID){
		case ShaderOutput::SignalID::INTERLEAVED:
			m_buffer = 0;
			return SUCCESS;
		case ShaderOutput::SignalID::DEINTERLEAVED:
			m_buffer = BufferBase::Ptr(std::dynamic_pointer_cast<BufferBase>(InternalBuffer::create()));
			m_buffer->setFrequencyMode(BufferBase::FrequencyMode::DYNAMIC);
			m_buffer->setNatureMode(BufferBase::NatureMode::COPY);
			return SUCCESS;
		}
	}
	return FAILURE;
}
