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

#include <UniShader/Utility.h>
#include <iostream>
#include <sstream>

#include "Buffer.h"

UNISHADER_BEGIN

template <typename T> 
Buffer<T>::Buffer(){

}

template <typename T>
const std::string& Buffer<T>::getClassName() const{
	static const std::string name("us::Buffer");
	return name;
}

template <typename T> 
Buffer<T>::~Buffer(){

}

template <typename T>
typename Buffer<T>::Ptr
Buffer<T>::create(){
	Ptr ptr(new Buffer);
	if(ptr->getGlID() == 0){
		return 0;
	}

	return ptr;
}

template <typename T> 
bool Buffer<T>::getDataCopy(std::vector<T>& vec) const{
	unsigned int tempSize = m_byteSize/sizeof(T);

	if(vec.size() != 0){
		std::cerr << "ERROR: Passed vector must be empty" << std::endl;
		return FAILURE;
	}

	void* mappedPtr = 0;
	if(!BufferBase::mapBuffer((void**)&mappedPtr)){
		std::cerr << "ERROR: Failed to access GPU memory" << std::endl;
		return FAILURE;
	}
	
	try{
		vec.resize(tempSize);
	}
	catch(...){
		std::cerr << "ERROR: Failed to allocate memory for data" << std::endl;
		mappedPtr = 0;
		vec.clear();
		BufferBase::unmapBuffer();
		return FAILURE;
	}

	//copy data to new memory space
	memcpy(&vec[0], mappedPtr, m_byteSize);

	return BufferBase::unmapBuffer();
}

template <typename T> 
bool Buffer<T>::getDataCopy(T*& arr, unsigned int& size) const{
	unsigned int tempSize = m_byteSize/sizeof(T);
	
	if(arr != 0){
		std::cerr << "ERROR: Passed pointer must be NULL" << std::endl;
		return FAILURE;
	}

	void* mappedPtr = 0;
	if(!BufferBase::mapBuffer((void**)&mappedPtr)){
		std::cerr << "ERROR: Failed to access GPU memory" << std::endl;
		return FAILURE;
	}
	
	try{
		arr = new T[tempSize];
	}
	catch(...){
		std::cerr << "ERROR: Failed to allocate memory for data" << std::endl;
		mappedPtr = 0;
		size = 0;
		BufferBase::unmapBuffer();
		return FAILURE;
	}

	//copy data to new memory space
	memcpy(arr, mappedPtr, m_byteSize);
	size = tempSize;

	return BufferBase::unmapBuffer();
}

template <typename T> 
std::string Buffer<T>::getString(unsigned int begin, unsigned int count, const std::ios& format) const{
	T* mappedPtr = 0;
	if(!BufferBase::mapBuffer((void**)&mappedPtr)){
		std::cerr << "ERROR: Failed to access GPU memory" << std::endl;
		return "";
	}

	unsigned int fieldSize = m_byteSize/sizeof(T);
	unsigned int end;

    std::ostringstream oss;

    oss.flags(format.flags());
    oss.precision(format.precision());
    oss.width(format.width());
    
	if(count == 0)
		end = fieldSize;
	else{
		if(begin+count > fieldSize)
			end = fieldSize;
		else
			end = begin+count;
	}

	for(unsigned int i = begin; i < end; i++){
		oss << *(mappedPtr+i) << " ";
	}

	mappedPtr = 0;
	BufferBase::unmapBuffer();

	return oss.str();
}

template <typename T> 
bool Buffer<T>::setData(const std::vector<T>& vec){
	if(vec.size() == 0){
		std::cerr << "ERROR: Zero sized vector passed, use 'clear' to clear data" << std::endl;
		return FAILURE;
	}
	if(BufferBase::setPlainData(&vec[0], sizeof(T)*vec.size()))
		return SUCCESS;
	else
		return FAILURE;
}

template <typename T> 
bool Buffer<T>::setData(const T* arr, unsigned int size){
	if(size == 0){
		std::cerr << "ERROR: Zero sized array passed, use 'clear' to clear data" << std::endl;
		return FAILURE;
	}
	if(BufferBase::setPlainData(arr, sizeof(T)*size))
		return SUCCESS;
	else
		return FAILURE;
}

UNISHADER_END
