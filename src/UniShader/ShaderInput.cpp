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

#include <UniShader/ShaderInput.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/OpenGL.h>
#include <UniShader/Attribute.h>
#include <UniShader/Uniform.h>

using UNISHADER_NAMESPACE;

ShaderInput::ShaderInput(ShaderProgram& program):
m_program(program),
m_VAO(0),
m_remakeVAO(true),
m_active(false){
	m_program.subscribeReceiver(signalPtr);
}

const std::string& ShaderInput::getClassName() const{
	static const std::string name("us::ShaderInput");
	return name;
}

ShaderInput::~ShaderInput(){
	glDeleteVertexArrays(1, &m_VAO);
	m_program.unsubscribeReceiver(signalPtr);
}

Attribute::Ptr ShaderInput::addAttribute(std::string name){
	for(std::deque< std::shared_ptr<Attribute> >::iterator it = m_attribs.begin(); it != m_attribs.end(); it++){
		if((*it)->getName() == name)
			return Attribute::Ptr();
	}
	m_attribs.push_back( std::shared_ptr<Attribute>(new Attribute(m_program, name)) );
	m_attribs.back()->subscribeReceiver(signalPtr);
	m_remakeVAO = true;
	return m_attribs.back();
}

Uniform::Ptr ShaderInput::addUniform(std::string name){
	for(std::deque< std::shared_ptr<Uniform> >::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++){
		if((*it)->getName() == name)
			return Uniform::Ptr();
	}
	m_uniforms.push_back( std::shared_ptr<Uniform>(new Uniform(m_program, name)) );
	return m_uniforms.back();
}

Attribute::Ptr ShaderInput::getAttribute(std::string name){
	for(std::deque< std::shared_ptr<Attribute> >::iterator it = m_attribs.begin(); it != m_attribs.end(); it++){
		if((*it)->getName() == name)
			return (*it);
	}
	return Attribute::Ptr();
}

Uniform::Ptr ShaderInput::getUniform(std::string name){
	for(std::deque< std::shared_ptr<Uniform> >::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++){
		if((*it)->getName() == name)
			return (*it);
	}
	return Uniform::Ptr();
}

void ShaderInput::removeAttribute(std::string name){
	for(std::deque< std::shared_ptr<Attribute> >::iterator it = m_attribs.begin(); it != m_attribs.end(); it++){
		if((*it)->getName() == name){
			(*it)->unsubscribeReceiver(signalPtr);
			m_attribs.erase(it);
			m_remakeVAO = true;
			return;
		}
	}
}

void ShaderInput::removeUniform(std::string name){
	for(std::deque< std::shared_ptr<Uniform> >::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++){
		if((*it)->getName() == name){
			m_uniforms.erase(it);
			return;
		}
	}
}

void ShaderInput::prepare(){
	if(m_program.getLinkStatus() != ShaderProgram::LinkStatus::SUCCESSFUL_LINK){
		std::cerr << "ERROR: Shader program is not linked" << std::endl;
		return;
	}

	if(m_remakeVAO){
		ensureGlewInit();
		clearGLErrors();
		glDeleteVertexArrays(1, &m_VAO);
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
		printGLError();

		//apply attributes to VAO
		for(std::deque< std::shared_ptr<Attribute> >::iterator it = m_attribs.begin(); it != m_attribs.end(); it++)
			(*it)->apply();

		glBindVertexArray(0);
	}
}

void ShaderInput::activate(){
	if(!m_active){
		ensureGlewInit();
		clearGLErrors();
		//apply uniforms to program
		for(std::deque< std::shared_ptr<Uniform> >::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++)
			(*it)->apply();
		glBindVertexArray(m_VAO);
		printGLError();
		m_active = true;
	}
}

bool ShaderInput::isActive(){
	return m_active;
}

void ShaderInput::deactivate(){
	if(m_active){
		ensureGlewInit();
		clearGLErrors();
		glBindVertexArray(0);
		for(std::deque< std::shared_ptr<Uniform> >::iterator it = m_uniforms.begin(); it != m_uniforms.end(); it++)
			(*it)->deactivateTextureSource();
		m_active = false;
	}
}

bool ShaderInput::handleSignal(unsigned int signalID, const ObjectBase* callerPtr){
	if(callerPtr->getClassName() == "us::Attribute"){
		switch(signalID){
		case Attribute::SignalID::CHANGED:
			m_remakeVAO = true;
			return SUCCESS;
		}
	}
	else if(callerPtr->getClassName() == "us::ShaderProgram"){
		switch(signalID){
		case ShaderProgram::SignalID::RELINKED:
			m_remakeVAO = true;
			return SUCCESS;
		}
	}
	return FAILURE;
}
