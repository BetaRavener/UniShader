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

#include <UniShader/ShaderProgram.h>
#include <UniShader/UniShader.h>
#include <UniShader/OpenGL.h>
#include <UniShader/ShaderObject.h>
#include <UniShader/ShaderInput.h>
#include <UniShader/ShaderOutput.h>

#include <iostream>
#include <string>

using UNISHADER_NAMESPACE;

ShaderProgram::ShaderProgram():
m_programObjectID(0),
m_linkStatus(LinkStatus::NONE),
m_active(false){
	m_input = std::shared_ptr<ShaderInput>(new ShaderInput(*this));
	m_output = std::shared_ptr<ShaderOutput>(new ShaderOutput(*this));
}

const std::string& ShaderProgram::getClassName() const{
	static const std::string name("us::ShaderProgram");
	return name;
}

ShaderProgram::~ShaderProgram(){;
	clearGLErrors();

	glDeleteProgram(m_programObjectID);
	printGLError();
}

ShaderProgram::Ptr ShaderProgram::create(){
	clearGLErrors();

	Ptr ptr ( new ShaderProgram );

	printGLError();
	return ptr;
}

void ShaderProgram::addShaderObject(ShaderObject::Ptr& shaderObjPtr){
	for(std::deque< ShaderObject::Ptr >::iterator it = m_shaderObjects.begin(); it != m_shaderObjects.end(); it++){
		if((*it)->getGlID() == shaderObjPtr->getGlID())
			return;
	}
	m_linkStatus = LinkStatus::PENDING_LINK;
	m_shaderObjects.push_back(shaderObjPtr);
	shaderObjPtr->subscribeReceiver(signalPtr);
}

bool ShaderProgram::removeShaderObject(ShaderObject::Ptr& shaderObjPtr){
	for(std::deque<ShaderObject::Ptr>::iterator it = m_shaderObjects.begin(); it != m_shaderObjects.end(); it++){
		if((*it) == shaderObjPtr){
			m_linkStatus = LinkStatus::PENDING_LINK;
			(*it)->unsubscribeReceiver(signalPtr);
			m_shaderObjects.erase(it);
			return SUCCESS;
		}
	}
	return FAILURE;
}

SafePtr<ShaderInput> ShaderProgram::getInput(){
	return m_input;
}

SafePtr<ShaderOutput> ShaderProgram::getOutput(){
	return m_output;
}

unsigned int ShaderProgram::getGlID() const{
	return m_programObjectID;
}

ShaderProgram::LinkStatus ShaderProgram::getLinkStatus() const{
	return m_linkStatus;
}

bool ShaderProgram::ensureLink(){
	if(m_linkStatus == LinkStatus::PENDING_LINK){
		if(link()){
			m_linkStatus = LinkStatus::SUCCESSFUL_LINK;
			return SUCCESS;
		}
		else{
			m_linkStatus = LinkStatus::FAILED_LINK;
			return FAILURE;
		}
	}
	else{
		if(m_linkStatus == LinkStatus::SUCCESSFUL_LINK)
			return SUCCESS;
		else
			return FAILURE;
	}
}

bool ShaderProgram::activate(){
	if(!m_active){
		clearGLErrors();

		ensureLink();
	
		m_input->prepare();

		glUseProgram(m_programObjectID);
		if(printGLError()){
			glUseProgram(0);
			return FAILURE;
		}

		m_input->activate();

		m_active = true;
		return SUCCESS;
	}
	return FAILURE;
}

bool ShaderProgram::activate(PrimitiveType primitiveType, unsigned int primitiveCount){
	if(!m_active){
		clearGLErrors();

		ensureLink();
	
		m_input->prepare();
		m_output->prepare(primitiveCount);

		glUseProgram(m_programObjectID);
		if(printGLError()){
			glUseProgram(0);
			return FAILURE;
		}

		m_input->activate();
		m_output->activate(primitiveType);

		m_active = true;
		return SUCCESS;
	}
	return FAILURE;
}

bool ShaderProgram::isActive(){
	return m_active;
}

bool ShaderProgram::deactivate(){
	if(m_active){
		clearGLErrors();

		m_output->deactivate();

		m_input->deactivate();
	
		glUseProgram(0);
		m_active = false;
		return !printGLError();
	}
	return FAILURE;
}

bool ShaderProgram::handleSignal(unsigned int signalID, const ObjectBase* callerPtr){
	if(callerPtr->getClassName() == "us::ShaderObject"){
		switch(signalID){
		case ShaderObject::SignalID::CHANGED:
		case ShaderObject::SignalID::RECOMPILED:
			m_linkStatus = LinkStatus::PENDING_LINK;
			return SUCCESS;
		}
	}
	else if(callerPtr->getClassName() == "us::ShaderOutput"){
		switch(signalID){
		case ShaderOutput::SignalID::CHANGED:
			m_linkStatus = LinkStatus::PENDING_LINK;
			return SUCCESS;
		}
	}
	return FAILURE;
}

bool ShaderProgram::link(){
	clearGLErrors();

	//Recreate program object because
	//it is safer to make new rather than
	//change old one due to driver bugs
	if(glIsProgram(m_programObjectID))
		glDeleteProgram(m_programObjectID);
	m_programObjectID = glCreateProgram();
	if(printGLError()){
		std::cerr << "ERROR: Failed to create shader program" << std::endl;
		return FAILURE;
	}

	m_output->setUp();

	bool compiledShaderPressent = false;
	for(unsigned int i = 0; i < m_shaderObjects.size(); i++){
		if(m_shaderObjects[i]->ensureCompilation()){
			if(m_shaderObjects[i]->getType() == us::ShaderObject::Type::FRAGMENT || m_shaderObjects[i]->getType() == us::ShaderObject::Type::VERTEX)
				compiledShaderPressent = true;

			glAttachShader(m_programObjectID, m_shaderObjects[i]->getGlID());
			printGLError();
		}
	}
	if(!compiledShaderPressent){
		std::cerr << "ERROR: No compiled shader object within program." << std::endl;
		return FAILURE;
	}

    // Link the program object and print out the info log
    glLinkProgram(m_programObjectID);
    printGLError();
	GLint linkStatus;
    glGetProgramiv(m_programObjectID, GL_LINK_STATUS, &linkStatus);
	printProgramInfoLog();

	if(linkStatus == GL_TRUE){
		sendSignal(SignalID::RELINKED, this);
		return SUCCESS;
	}
	else{
		std::cerr << "ERROR: Shader program link failed"<< std::endl;
		return FAILURE;
	}
}

int ShaderProgram::printProgramInfoLog() const{
	clearGLErrors();

    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetProgramiv(m_programObjectID, GL_INFO_LOG_LENGTH, &infologLength);
	printGLError(); 

    if(infologLength > 0){
        infoLog = (char *)malloc(infologLength);
        if(infoLog == NULL){
			std::cerr << "ERROR: Could not allocate InfoLog buffer" << std::endl;
            return 1;
        }

        glGetProgramInfoLog(m_programObjectID, infologLength, &charsWritten, infoLog);
		printGLError();

		std::cout << "Program InfoLog:" << std::endl << infoLog <<std::endl<<std::endl;
        free(infoLog);
    }
	return 0;
}
