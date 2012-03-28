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

#include <UniShader/Uniform.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/TextureBuffer.h>
#include <UniShader/OpenGL.h>
#include <UniShader/TypeResolver.h>

#include <cstring>

using UNISHADER_NAMESPACE;

Uniform::Uniform(ShaderProgram& program, std::string name):
m_program(program),
m_name(name),
m_textureBuffer(0),
m_plainData(0),
m_dataByteSize(0),
m_location(-1),
m_transposeMatrix(false),
m_prepared(false),
m_applied(false){
	m_program.subscribeReceiver(signalPtr);
}

const std::string& Uniform::getClassName() const{
	static const std::string name("us::Uniform");
	return name;
}

Uniform::~Uniform(){
	m_program.unsubscribeReceiver(signalPtr);
}

std::string Uniform::getName() const{
	return m_name;
}

void Uniform::transposeMatrix(bool transpose){
	m_transposeMatrix = transpose;
	m_applied = false;
}

bool Uniform::isMatrixTransposed(){
	return m_transposeMatrix;
}

void Uniform::clearSource(){
	if(m_plainData){
		delete m_plainData;
		m_plainData = 0;
		m_dataByteSize = 0;
	}
	else if(m_textureBuffer){
		m_textureBuffer = 0;
	}
}

void Uniform::setSource(float* arr, unsigned int size){
	clearSource();
	m_dataByteSize = size*sizeof(float);
	m_plainData = (char*)new float[size];
	memcpy(m_plainData, &arr[0], m_dataByteSize);
}

void Uniform::setSource(std::vector<float> vec){
	clearSource();
	unsigned int size = vec.size();
	m_dataByteSize = size*sizeof(float);
	m_plainData = (char*)new float[size];
	memcpy(m_plainData, &vec[0], m_dataByteSize);
}

void Uniform::setSource(TextureBuffer::Ptr& textureBuffer){
	clearSource();
	m_textureBuffer = textureBuffer;
}

const GLSLType& Uniform::getGLSLType() const{
	return m_type;
}

bool Uniform::prepare(){
	ensureGlewInit();
	clearGLErrors();

	if(m_program.getLinkStatus() != ShaderProgram::LinkStatus::SUCCESSFUL_LINK){
		std::cerr << "ERROR: Shader program is not linked" << std::endl;
		return FAILURE;
	}

	if(!m_prepared){
		m_location = glGetUniformLocation(m_program.getGlID(), m_name.c_str());
	
		printGLError();
		if(m_location == -1){
			std::cerr << "ERROR: Uniform " << m_name <<  " doesn't exist in program" << std::endl;
			return FAILURE;
		}

		GLsizei charSize = m_name.size()+1, size = 0, length = 0;
		GLenum type = 0;
		GLchar* name = new GLchar[charSize];
		name[charSize-1] = '\0';
		glGetActiveUniform(m_program.getGlID(), m_location, charSize, &length, &size, &type, name); 

		delete name; name = 0;

		if(!TypeResolver::resolve(type, m_type))
			return FAILURE;

		m_prepared = true;
		m_applied = false;
	}

	return SUCCESS;
}

void Uniform::apply(){
	ensureGlewInit();
	clearGLErrors();

	if(!prepare())
		return;

	//textures must be applied everytime because texturing unit may change
	if(!m_applied || m_textureBuffer){
		//use correct version to set uniform
		switch(m_type.getObjectType()){
		case GLSLType::ObjectType::VALUE:
			if(!m_plainData){
				std::cerr << "ERROR: Uniform's source isn't value";
				return;
			}
			switch(m_type.getDataType()){

				//integer datatypes
			case GLSLType::DataType::INT:{
				if(m_type.getColumnCount() != 1){
					std::cerr << "ERROR: Invalid column count, integers can have only one column" << std::endl;
					return;
				}
				switch(m_type.getColumnSize()){
				case 1:
					glUniform1iv(m_location, 1, (const GLint*)m_plainData);
					break;
				case 2:
					glUniform2iv(m_location, 1, (const GLint*)m_plainData);
					break;
				case 3:
					glUniform3iv(m_location, 1, (const GLint*)m_plainData);
					break;
				case 4:
					glUniform4iv(m_location, 1, (const GLint*)m_plainData);
					break;
				default:
					std::cerr << "ERROR: Invalid column size" << std::endl;
					return;
				}
				}
				break;

				//unsigned integer datatypes
			case GLSLType::DataType::UNSIGNED_INT:{
				if(m_type.getColumnCount() != 1){
					std::cerr << "ERROR: Invalid column count, integers can have only one column" << std::endl;
					return;
				}
				switch(m_type.getColumnSize()){
				case 1:
					glUniform1uiv(m_location, 1, (const GLuint*)m_plainData);
					break;
				case 2:
					glUniform2uiv(m_location, 1, (const GLuint*)m_plainData);
					break;
				case 3:
					glUniform3uiv(m_location, 1, (const GLuint*)m_plainData);
					break;
				case 4:
					glUniform4uiv(m_location, 1, (const GLuint*)m_plainData);
					break;
				default:
					std::cerr << "ERROR: Invalid column size" << std::endl;
					return;
				}
				}
				break;

				//single precision floating point datatypes
			case GLSLType::DataType::FLOAT:
				switch(m_type.getColumnCount()){
				case 1:
					switch(m_type.getColumnSize()){
					case 1:
						glUniform1fv(m_location, 1, (const GLfloat*)m_plainData);
						break;
					case 2:
						glUniform2fv(m_location, 1, (const GLfloat*)m_plainData);
						break;
					case 3:
						glUniform3fv(m_location, 1, (const GLfloat*)m_plainData);
						break;
					case 4:
						glUniform4fv(m_location, 1, (const GLfloat*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 2:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix2fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 3:
						glUniformMatrix2x3fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 4:
						glUniformMatrix2x4fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 3:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix3x2fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 3:
						glUniformMatrix3fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 4:
						glUniformMatrix3x4fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 4:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix4x2fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 3:
						glUniformMatrix4x3fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					case 4:
						glUniformMatrix4fv(m_location, 1, m_transposeMatrix, (const GLfloat*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				default:
					std::cerr << "ERROR: Invalid column count" << std::endl;
					return;
				}
				break;

				//double precision floating point datatypes
			case GLSLType::DataType::DOUBLE:
				switch(m_type.getColumnCount()){
				case 1:
					switch(m_type.getColumnSize()){
					case 1:
						glUniform1dv(m_location, 1, (const GLdouble*)m_plainData);
						break;
					case 2:
						glUniform2dv(m_location, 1, (const GLdouble*)m_plainData);
						break;
					case 3:
						glUniform3dv(m_location, 1, (const GLdouble*)m_plainData);
						break;
					case 4:
						glUniform4dv(m_location, 1, (const GLdouble*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 2:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix2dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 3:
						glUniformMatrix2x3dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 4:
						glUniformMatrix2x4dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 3:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix3x2dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 3:
						glUniformMatrix3dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 4:
						glUniformMatrix3x4dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				case 4:
					switch(m_type.getColumnSize()){
					case 2:
						glUniformMatrix4x2dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 3:
						glUniformMatrix4x3dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					case 4:
						glUniformMatrix4dv(m_location, 1, m_transposeMatrix, (const GLdouble*)m_plainData);
						break;
					default:
						std::cerr << "ERROR: Invalid column size" << std::endl;
						return;
					}
					break;
				default:
					std::cerr << "ERROR: Invalid column count" << std::endl;
					return;
				}
				break;

			default:
				std::cerr << "ERROR: Invalid data type" << std::endl;
				return;
			}
			break;

		case GLSLType::ObjectType::SAMPLER:
			if(!m_textureBuffer){
				std::cerr << "ERROR: Uniform's source isn't texture buffer" << std::endl;
				return;
			}
			
			switch(m_type.getSamplerType()){
			case GLSLType::SamplerType::BUFFER:
				//TODO: check sampler and buffer data types
				m_textureBuffer->activate();
				glUniform1i(m_location, m_textureBuffer->getTextureUnitIndex());
				break;
			default:
				std::cerr << "ERROR: Invalid sampler type" << std::endl;
				return;
			}
			break;

		default:
			std::cerr << "ERROR: Invalid object type" << std::endl;
			return;
		}
	}
	if(printGLError())
		return;
	m_applied = true;
}

void Uniform::deactivateTextureSource(){
	if(m_textureBuffer)
		m_textureBuffer->deactivate();
}

bool Uniform::handleSignal(unsigned int signalID, const ObjectBase* callerPtr){
	if(callerPtr->getClassName() == "us::ShaderProgram"){
		switch(signalID){
		case ShaderProgram::SignalID::RELINKED:
			m_prepared = false;
			return SUCCESS;
		}
	}
	return FAILURE;
}
