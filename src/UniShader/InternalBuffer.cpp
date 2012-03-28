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

#include <UniShader/InternalBuffer.h>

using UNISHADER_NAMESPACE;

InternalBuffer::InternalBuffer(){

}

const std::string& InternalBuffer::getClassName() const{
	static const std::string name("us::InternalBuffer");
	return name;
}

InternalBuffer::~InternalBuffer(){

}

InternalBuffer::Ptr InternalBuffer::create(){
	Ptr ptr(new InternalBuffer);
	return ptr;
}

size_t InternalBuffer::getSize() const{
	return m_byteSize;
}

void InternalBuffer::resize(size_t size){
	setPlainData(0,size);
}
