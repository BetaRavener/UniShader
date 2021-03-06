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

#include <UniShader/Uniform.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/Texture.h>
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
		delete[] m_plainData;
		m_plainData = 0;
		m_dataByteSize = 0;
	}
	else if(m_textureBuffer){
		m_textureBuffer = 0;
	}
	m_prepared = false;
}

void Uniform::setSource(float val){
	clearSource();
	m_dataByteSize = sizeof(float);
	m_plainData = (char*)new float[1];
	memcpy(m_plainData, &val, m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(int val){
	clearSource();
	m_dataByteSize = sizeof(int);
	m_plainData = (char*)new int[1];
	memcpy(m_plainData, &val, m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(unsigned int val){
	clearSource();
	m_dataByteSize = sizeof(unsigned int);
	m_plainData = (char*)new unsigned int[1];
	memcpy(m_plainData, &val, m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const float* arr, unsigned int size){
	clearSource();
	m_dataByteSize = size*sizeof(float);
	m_plainData = (char*)new float[size];
	memcpy(m_plainData, &arr[0], m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const int* arr, unsigned int size){
	clearSource();
	m_dataByteSize = size*sizeof(int);
	m_plainData = (char*)new int[size];
	memcpy(m_plainData, &arr[0], m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const unsigned int* arr, unsigned int size){
	clearSource();
	m_dataByteSize = size*sizeof(unsigned int);
	m_plainData = (char*)new unsigned int[size];
	memcpy(m_plainData, &arr[0], m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const std::vector<float>& vec){
	clearSource();
	unsigned int size = vec.size();
	m_dataByteSize = size*sizeof(float);
	m_plainData = (char*)new float[size];
	memcpy(m_plainData, &vec[0], m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const std::vector<int>& vec){
	clearSource();
	unsigned int size = vec.size();
	m_dataByteSize = size*sizeof(int);
	m_plainData = (char*)new int[size];
	memcpy(m_plainData, &vec[0], m_dataByteSize);
	m_prepared = false;
}

void Uniform::setSource(const std::vector<unsigned int>& vec){
	clearSource();
	unsigned int size = vec.size();
	m_dataByteSize = size*sizeof(unsigned int);
	m_plainData = (char*)new unsigned int[size];
	memcpy(m_plainData, &vec[0], m_dataByteSize);
    m_prepared = false;
}

void Uniform::setSource(Texture::Ptr& texture)
{
    clearSource();
    m_texture = texture;
    m_prepared = false;
}

void Uniform::setSource(TextureBuffer::Ptr& textureBuffer){
	clearSource();
	m_textureBuffer = textureBuffer;
	m_prepared = false;
}

const GLSLType& Uniform::getGLSLType() const{
	return m_type;
}

bool Uniform::prepare(){
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

		GLint uniformCount;
		glGetProgramiv(m_program.getGlID(),GL_ACTIVE_UNIFORMS, &uniformCount);
		for(int i = 0; i < uniformCount; i++){
			glGetActiveUniform(m_program.getGlID(), i, charSize, &length, &size, &type, name);
			if(length == m_name.size()){
				//if names match break search
				if(memcmp(name,m_name.c_str(),length) == 0)
					break;
			}
		}

		delete[] name; name = 0;

		if(!TypeResolver::resolve(type, m_type))
			return FAILURE;

		m_prepared = true;
		m_applied = false;
	}

	return SUCCESS;
}

void Uniform::apply(){
	clearGLErrors();

	if(!prepare())
		return;

	//textures must be applied everytime because texturing unit may change
    if(!m_applied || m_texture || m_textureBuffer){
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
            if(!m_texture && !m_textureBuffer){
                std::cerr << "ERROR: Uniform's source must be texture or texture buffer" << std::endl;
				return;
			}
			
            switch(m_type.getSamplerType()){
            case GLSLType::SamplerType::ONE_DIMENSIONAL:
                if (m_texture)
                {
                    if (m_texture->getType() != Texture::TextureType::ONE_DIM)
                    {
                        std::cerr << "ERROR: Texture sampler and texture have different dimensions";
                        return;
                    }

                    m_texture->activate();
                    glUniform1i(m_location, m_texture->getTextureUnitIndex());
                }
                else
                {
                    std::cerr << "ERROR: Uniform is texture sampler but no texture bound";
                    return;
                }
                break;
            case GLSLType::SamplerType::TWO_DIMENSIONAL:
                if (m_texture)
                {
                    if (m_texture->getType() != Texture::TextureType::TWO_DIM)
                    {
                        std::cerr << "ERROR: Texture sampler and texture have different dimensions";
                        return;
                    }

                    m_texture->activate();
                    glUniform1i(m_location, m_texture->getTextureUnitIndex());
                }
                else
                {
                    std::cerr << "ERROR: Uniform is texture sampler but no texture bound";
                    return;
                }
                break;
            case GLSLType::SamplerType::BUFFER:
                if (m_textureBuffer)
                {
                    //TODO: check sampler and buffer data types
                    m_textureBuffer->activate();
                    glUniform1i(m_location, m_textureBuffer->getTextureUnitIndex());
                }
                else
                {
                    std::cerr << "ERROR: Uniform is buffer sampler but no buffer bound";
                    return;
                }
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
