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

#include <UniShader/GLSLType.h>

using UNISHADER_NAMESPACE;

GLSLType::GLSLType(){
	clear();
}

GLSLType::GLSLType(const GLSLType& ref):
m_objectType(ref.m_objectType),
m_dataType(ref.m_dataType),
m_columnSize(ref.m_columnSize),
m_columnCount(ref.m_columnCount),
m_samplerType(ref.m_samplerType){

}

GLSLType::ObjectType GLSLType::getObjectType() const{
	return m_objectType;
}

GLSLType::DataType GLSLType::getDataType() const{
	return m_dataType;
}

unsigned char GLSLType::getColumnSize() const{
	return m_columnSize;
}

unsigned char GLSLType::getColumnCount() const{
	return m_columnCount;
}

GLSLType::SamplerType GLSLType::getSamplerType() const{
	return m_samplerType;
}

void us::GLSLType::clear(){
	m_objectType = ObjectType::NONE;
	m_dataType = DataType::NONE;
	m_columnSize = 0;
	m_columnCount = 0;
	m_samplerType = SamplerType::NONE;
}
