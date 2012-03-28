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
