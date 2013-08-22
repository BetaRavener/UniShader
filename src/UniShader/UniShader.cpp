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

#include <UniShader/UniShader.h>
#include <UniShader/OpenGL.h>

using UNISHADER_NAMESPACE;

//TODO: for whole application, querry max values
//affects trnasform feedback separate, attributes,
//buffer sizes, texture sizes, samplers

//TODO: use bool return to stop something messy going on

//We are using C0x now for shared_ptr and weak_ptr, HELL YEAAAH (just to remind you to download new compiler) 
//and also those enum classes, but they will be introduced in VS11 so we for now we are using ugly classes

UniShader::UniShader():
m_program(0){

}

UniShader::~UniShader(){
}

void UniShader::connectProgram(ShaderProgram::Ptr& program){
	m_program = program;
}

ShaderProgram::Ptr UniShader::program()
{
	return m_program;
}

void UniShader::disconnectProgram(){
	m_program = 0;
}

void UniShader::render(PrimitiveType primitiveType, unsigned int primitiveCount, unsigned int offset, bool record, bool wait){
	if(!m_program){
		std::cerr << "ERROR: No shader program connected." << std::endl;
		return;
	}
	ensureGlewInit();
	clearGLErrors();

	GLenum mode;
	
	switch(primitiveType){
	case PrimitiveType::POINTS:
		mode = GL_POINTS;
		break;
	case PrimitiveType::LINES:
		mode = GL_LINES;
		break;
	case PrimitiveType::LINE_STRIP:
		mode = GL_LINE_STRIP;
		break;
	case PrimitiveType::TRIANGLES:
		mode = GL_TRIANGLES;
		break;
	case PrimitiveType::TRIANGLE_STRIP:
		mode = GL_TRIANGLE_STRIP;
		break;
	default:
		std::cerr << "ERROR: Invalid primitive type" << std::endl;
		return;
	}

	if(record)
		m_program->activate(primitiveType, primitiveCount);
	else
		m_program->activate();

	glDrawArrays(mode, offset, primitiveCount);

	if(wait)
		glFinish();
	printGLError();
	m_program->deactivate();
}

void UniShader::renderElements(Buffer<unsigned int>::Ptr elementsBuffer, PrimitiveType primitiveType, unsigned int primitiveCount, unsigned int offset, bool record, bool wait){
	if(!m_program){
		std::cerr << "ERROR: No shader program connected." << std::endl;
		return;
	}
	ensureGlewInit();
	clearGLErrors();

	GLenum mode;
	
	switch(primitiveType){
	case PrimitiveType::POINTS:
		mode = GL_POINTS;
		break;
	case PrimitiveType::LINES:
		mode = GL_LINES;
		break;
	case PrimitiveType::LINE_STRIP:
		mode = GL_LINE_STRIP;
		break;
	case PrimitiveType::TRIANGLES:
		mode = GL_TRIANGLES;
		break;
	case PrimitiveType::TRIANGLE_STRIP:
		mode = GL_TRIANGLE_STRIP;
		break;
	default:
		std::cerr << "ERROR: Invalid primitive type" << std::endl;
		return;
	}

	if(record)
		m_program->activate(primitiveType, primitiveCount);
	else
		m_program->activate();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer->getGlID());
	glDrawRangeElements(mode, offset, offset + primitiveCount, primitiveCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if(wait)
		glFinish();
	printGLError();
	m_program->deactivate();
}