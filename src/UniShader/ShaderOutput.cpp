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

#include <UniShader/ShaderOutput.h>
#include <UniShader/UniShader.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/InternalBuffer.h>
#include <UniShader/OpenGL.h>

#include <cstring>

using UNISHADER_NAMESPACE;

ShaderOutput::ShaderOutput(ShaderProgram& program):
m_program(program),
m_interleavedBuffer(0),
m_overallSize(0),
m_query(0),
m_primitivesProcessed(0),
m_interleaved(false),
m_prepared(false),
m_active(false){

}

const std::string& ShaderOutput::getClassName() const{
	static const std::string name("us::ShaderOutput");
	return name;
}

ShaderOutput::~ShaderOutput(){

}

Varying::Ptr ShaderOutput::addVarying(std::string name){
	for(std::deque< std::shared_ptr<Varying> >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++){
		if((*it)->getName() == name)
			return Varying::Ptr();
	}
	m_varyings.push_back( std::shared_ptr<Varying>(new Varying(m_program, *this, name)) );

	size_t size = m_varyings.back()->getName().size()+1;
	m_names.push_back(new char[size]);
	memcpy((void*)m_names.back(), m_varyings.back()->getName().c_str(), size);

	m_prepared = false;
	sendSignal(SignalID::CHANGED, this);
	return m_varyings.back();
}

Varying::Ptr ShaderOutput::getVarying(std::string name){
	for(std::deque< std::shared_ptr<Varying> >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++){
		if((*it)->getName() == name)
			return (*it);
	}
	return Varying::Ptr();
}

void ShaderOutput::removeVarying(std::string name){
	unsigned int i = 0;
	for(std::deque< std::shared_ptr<Varying>  >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++, i++){
		if((*it)->getName() == name){
			m_varyings.erase(it);

			delete (*(m_names.begin()+i));
			(*(m_names.begin()+i)) = 0;
			m_names.erase(m_names.begin()+i);

			m_prepared = false;
			sendSignal(SignalID::CHANGED, this);
			return;
		}
	}
}

void ShaderOutput::interleave(bool interl){
	if(interl != m_interleaved){
		m_interleaved = interl;
		if(m_interleaved){
			m_interleavedBuffer = BufferBase::Ptr(std::dynamic_pointer_cast<BufferBase>(InternalBuffer::create()));
			m_interleavedBuffer->setFrequencyMode(BufferBase::FrequencyMode::DYNAMIC);
			m_interleavedBuffer->setNatureMode(BufferBase::NatureMode::COPY);
			sendSignal(SignalID::INTERLEAVED, this);
		}
		else{
			m_interleavedBuffer = 0;
			sendSignal(SignalID::DEINTERLEAVED, this);
		}
		m_prepared = false;
		sendSignal(SignalID::CHANGED, this);
	}
}

bool ShaderOutput::isInterleaved(){
	return m_interleaved;
}

void ShaderOutput::setUp(){
	if(m_varyings.size() == 0)
		return;

	ensureGlewInit();
	clearGLErrors();

	if(m_interleaved)
		glTransformFeedbackVaryings(m_program.getGlID(), m_names.size(), &m_names[0], GL_INTERLEAVED_ATTRIBS);
	else
		glTransformFeedbackVaryings(m_program.getGlID(), m_names.size(), &m_names[0], GL_SEPARATE_ATTRIBS);

	printGLError();
}

bool ShaderOutput::prepare(unsigned int primitiveCount){
	if(m_varyings.size() == 0)
		return SUCCESS;

	if(m_program.getLinkStatus() != ShaderProgram::LinkStatus::SUCCESSFUL_LINK){
		std::cerr << "ERROR: Shader program is not linked" << std::endl;
		return FAILURE;
	}

	if(m_interleaved){

		if(!m_prepared){
			size_t unitSize = 0;
			m_overallSize = 0;
			unsigned int i = 0;
			for(std::deque< std::shared_ptr<Varying> >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++, i++){
				(*it)->prepare(i, primitiveCount, &unitSize);
				m_overallSize += unitSize;
			}
			m_prepared = true;
		}

		size_t bufSize = m_overallSize * primitiveCount;
		if(bufSize != std::static_pointer_cast<InternalBuffer>(m_interleavedBuffer)->getSize() && bufSize != 0)
			std::static_pointer_cast<InternalBuffer>(m_interleavedBuffer)->resize(bufSize);
	}
	else{
		unsigned int i = 0;
		for(std::deque< std::shared_ptr<Varying> >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++, i++){
			(*it)->prepare(i, primitiveCount);
		}
	}

	return SUCCESS;
}

void ShaderOutput::activate(PrimitiveType primitiveType){
	if(m_varyings.size() == 0)
		return;

	if(!m_active){
		ensureGlewInit();
		clearGLErrors();

		m_primitivesProcessed = 0;

		if(m_interleaved){
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_interleavedBuffer->getGlID());
			printGLError();
		}
		else{
			unsigned int i = 0;
			for(std::deque< std::shared_ptr<Varying> >::iterator it = m_varyings.begin(); it != m_varyings.end(); it++, i++){
				glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, (*it)->getBuffer<void>()->getGlID());
				printGLError();
			}
		}
	
		GLenum primType = 0;
		switch(primitiveType){
		case PrimitiveType::POINTS:
			primType = GL_POINTS;
			break;
		case PrimitiveType::LINES:
		case PrimitiveType::LINE_STRIP:
			primType = GL_LINES;
			break;
		case PrimitiveType::TRIANGLES:
		case PrimitiveType::TRIANGLE_STRIP:
			primType = GL_TRIANGLES;
			break;
		default:
			std::cerr << "ERROR: Invalid primitive type" << std::endl;
            return;
		}
        
        glGenQueries(1, &m_query);
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_query);
		printGLError();

		glBeginTransformFeedback(primType);
		printGLError();

		m_active = true;
	}
}

bool ShaderOutput::isActive(){
	return m_active;
}

void ShaderOutput::deactivate(){
	if(m_active){
		ensureGlewInit();
		clearGLErrors();

		glEndTransformFeedback();
		glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
		glGetQueryObjectiv(m_query, GL_QUERY_RESULT, &m_primitivesProcessed);
		printGLError();

		if(m_interleaved){
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
			printGLError();
		}
		else{
			for(unsigned int i = 0; i < m_varyings.size(); i++){
				glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, 0);
				printGLError();
			}
		}
		m_active = false;
	}
}
