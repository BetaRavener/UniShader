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

#include <UniShader/TextureBuffer.h>
#include <UniShader/OpenGL.h>
#include <UniShader/Buffer.h>

#include <iostream>

using UNISHADER_NAMESPACE;

TextureBuffer::TextureBuffer():
m_buffer(0),
m_dataType(DataType::NONE),
m_texture(0),
m_activeCount(0),
m_componentsNumber(0),
m_prepared(false){
	clearGLErrors();

	glGenTextures(1, &m_texture);
	printGLError();
}

const std::string& TextureBuffer::getClassName() const{
	static const std::string name("us::TextureBuffer");
	return name;
}

TextureBuffer::~TextureBuffer(){
	glDeleteTextures(1, &m_texture);
	printGLError();
	m_unit.release();
}

TextureBuffer::Ptr TextureBuffer::create(){
	clearGLErrors();

	Ptr ptr(new TextureBuffer);
	if((ptr->m_texture == 0))
		return 0;

	ptr->m_unit.lock();
	if(!ptr->m_unit.makeActive()){
		ptr->m_unit.release();
		return 0;
	}
	//first bind is used to specialize texture
	glBindTexture(GL_TEXTURE_BUFFER, ptr->m_texture);
	glBindTexture(GL_TEXTURE_BUFFER, 0);

	//TODO: Unexpected texture problems may be caused by this
	ptr->m_unit.release();

	if(printGLError())
		return 0;

	return ptr;
}

void TextureBuffer::connectBuffer(BufferBase::Ptr buffer, unsigned char componentsNumber, DataType dataType){
	m_buffer = buffer;
	m_buffer->subscribeReceiver(signalPtr);
	m_componentsNumber = componentsNumber;
	m_dataType = dataType;
	m_prepared = false;
}

void TextureBuffer::disconnectBuffer(){
	m_buffer->unsubscribeReceiver(signalPtr);
	m_buffer = 0;
	m_componentsNumber = 0;
	m_dataType = DataType::NONE;
	m_prepared = false;
}

unsigned char TextureBuffer::getComponentsNumber() const{
	return m_componentsNumber;
}

TextureBuffer::DataType TextureBuffer::getDataType() const{
	return m_dataType;
}

char TextureBuffer::getTextureUnitIndex() const{
	if(m_activeCount == 0){
		std::cerr << "ERROR: Texture buffer must be activated before querying texture unit index" << std::endl;
		return -1;
	}
	else
		return m_unit.getIndex();
}

void TextureBuffer::setComponentsNumber(unsigned char componentsNumber){
	m_componentsNumber = componentsNumber;
	m_prepared = false;
}

void TextureBuffer::setDataType(DataType dataType){
	m_dataType = dataType;
	m_prepared = false;
}

bool TextureBuffer::prepare(){
	clearGLErrors();

	if(!m_prepared){
		m_unit.lock();
		if(!m_unit.makeActive())
			return FAILURE;

		glBindTexture(GL_TEXTURE_BUFFER, m_texture);
		GLenum internalFormat;

		switch(m_componentsNumber){
		case 1:
			switch(m_dataType){
			case DataType::CHAR:
				internalFormat = GL_R8I;
				break;
			case DataType::UNSIGNED_CHAR:
				internalFormat = GL_R8UI;
				break;
			case DataType::SHORT:
				internalFormat = GL_R16I;
				break;
			case DataType::UNSIGNED_SHORT:
				internalFormat = GL_R16UI;
				break;
			case DataType::HALF_FLOAT:
				internalFormat = GL_R16F;
				break;
			case DataType::INT:
				internalFormat = GL_R32I;
				break;
			case DataType::UNSIGNED_INT:
				internalFormat = GL_R32UI;
				break;
			case DataType::FLOAT:
				internalFormat = GL_R32F;
				break;
			default:
				std::cerr << "ERROR: Invalid data type" << std::endl;
				return FAILURE;
			}
			break;
		case 2:
			switch(m_dataType){
			case DataType::CHAR:
				internalFormat = GL_RG8I;
				break;
			case DataType::UNSIGNED_CHAR:
				internalFormat = GL_RG8UI;
				break;
			case DataType::SHORT:
				internalFormat = GL_RG16I;
				break;
			case DataType::UNSIGNED_SHORT:
				internalFormat = GL_RG16UI;
				break;
			case DataType::HALF_FLOAT:
				internalFormat = GL_RG16F;
				break;
			case DataType::INT:
				internalFormat = GL_RG32I;
				break;
			case DataType::UNSIGNED_INT:
				internalFormat = GL_RG32UI;
				break;
			case DataType::FLOAT:
				internalFormat = GL_RG32F;
				break;
			default:
				std::cerr << "ERROR: Invalid data type" << std::endl;
				return FAILURE;
			}
			break;
		case 3:
			switch(m_dataType){
			case DataType::CHAR:
				internalFormat = GL_RGB8I;
				break;
			case DataType::UNSIGNED_CHAR:
				internalFormat = GL_RGB8UI;
				break;
			case DataType::SHORT:
				internalFormat = GL_RGB16I;
				break;
			case DataType::UNSIGNED_SHORT:
				internalFormat = GL_RGB16UI;
				break;
			case DataType::HALF_FLOAT:
				internalFormat = GL_RGB16F;
				break;
			case DataType::INT:
				internalFormat = GL_RGB32I;
				break;
			case DataType::UNSIGNED_INT:
				internalFormat = GL_RGB32UI;
				break;
			case DataType::FLOAT:
				internalFormat = GL_RGB32F;
				break;
			default:
				std::cerr << "ERROR: Invalid data type" << std::endl;
				return FAILURE;
			}
			break;
		case 4:
			switch(m_dataType){
			case DataType::CHAR:
				internalFormat = GL_RGBA8I;
				break;
			case DataType::UNSIGNED_CHAR:
				internalFormat = GL_RGBA8UI;
				break;
			case DataType::SHORT:
				internalFormat = GL_RGBA16I;
				break;
			case DataType::UNSIGNED_SHORT:
				internalFormat = GL_RGBA16UI;
				break;
			case DataType::HALF_FLOAT:
				internalFormat = GL_RGBA16F;
				break;
			case DataType::INT:
				internalFormat = GL_RGBA32I;
				break;
			case DataType::UNSIGNED_INT:
				internalFormat = GL_RGBA32UI;
				break;
			case DataType::FLOAT:
				internalFormat = GL_RGBA32F;
				break;
			default:
				std::cerr << "ERROR: Invalid data type" << std::endl;
				return FAILURE;
			}
			break;
		default:
			std::cerr << "ERROR: Invalid number of components" << std::endl;
			return FAILURE;
		}

		if(m_buffer){
			glTexBuffer(GL_TEXTURE_BUFFER, internalFormat, m_buffer->getGlID());
			glBindTexture(GL_TEXTURE_BUFFER, 0);
			m_prepared = true;
		}
		else{
			std::cerr << "ERROR: Data buffer isn't connected" << std::endl;
			return FAILURE;
		}
		
	}

	return SUCCESS;
}

void TextureBuffer::activate(){
	clearGLErrors();

	if(!prepare())
		return;

	m_unit.lock();
	if(!m_unit.makeActive())
		return;
	glBindTexture(GL_TEXTURE_BUFFER, m_texture);
		
	m_activeCount++;
}

void TextureBuffer::deactivate(){
	clearGLErrors();

	if(m_activeCount != 0){
		m_activeCount--;
		if(m_activeCount == 0){
			m_unit.makeActive();
			glBindTexture(GL_TEXTURE_BUFFER, 0);
			m_unit.release();
		}
	}
}

bool TextureBuffer::handleSignal(unsigned int signalID, const ObjectBase* callerPtr)
{
	if(callerPtr->getClassName() == "us::Buffer"){
		switch(signalID){
		case BufferBase::SignalID::CHANGED:
			m_prepared = false;
			return SUCCESS;
		}
	}
	return FAILURE;
}
