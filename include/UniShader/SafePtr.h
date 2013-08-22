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

#pragma once
#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <UniShader/Config.h>
#include <UniShader/Utility.h>

#include <memory>

UNISHADER_BEGIN

//! Safe pointer class
/*!
	Safe pointer is wrapper arround weak pointer class.
	It allows to use weak pointers as shared pointes, however they
	use only weak counting. It is used to return pointer to inner
	interface object thats existence doesn't depend on outer 
	references to it. It is safe in a way that when object pointed to
	is destroyed, any dereferencing will result in error.
*/

template<class T>
class SafePtr : protected std::weak_ptr<T>{
public:
	SafePtr();
	SafePtr(const std::shared_ptr<T>& shared);

	//! Dereference pointer
	/*!
		\return Pointer to object
	*/
	T* operator->();

	//! Dereference pointer
	/*!
		\return Reference to object
	*/
	T& operator*();

	//! Initialize with shared pointer
	/*!
		\return Safe pointer to object
	*/
	SafePtr& operator=(const std::shared_ptr<T>& shared);

	//! Return object existence
	/*!
		\return True if object exists
	*/
	operator bool();
};

UNISHADER_END

#include <UniShader/SafePtr.inl>

#endif
