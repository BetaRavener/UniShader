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

#include <UniShader/Utility.h>

#include <cassert>

UNISHADER_BEGIN

template <typename T>
SafePtr<T>::SafePtr(){
	
}

template <typename T>
SafePtr<T>::SafePtr(const std::shared_ptr<T>& shared){
	std::weak_ptr<T>::operator=(shared);
}

template <typename T>
T* SafePtr<T>::operator->(){
	assert(!this->expired());
	return this->lock().get();
}

template <typename T>
T& SafePtr<T>::operator*(){
	assert(!this->expired());
	return *this->lock();
}

template <typename T>
SafePtr<T>& SafePtr<T>::operator=(const std::shared_ptr<T>& shared){
	std::weak_ptr<T>::operator=(shared);
	return *this;
}

template <typename T>
SafePtr<T>::operator bool(){
	return !this->expired();
}

UNISHADER_END
