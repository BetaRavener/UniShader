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
#ifndef INTERNAL_BUFFER_H
#define INTERNAL_BUFFER_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/Buffer.h>

UNISHADER_BEGIN

//! Internal buffer class.
/*!
	Internal buffer class is inner class used by interface.
*/

class InternalBuffer : public BufferBase{
private:
	InternalBuffer();
public:
	virtual const std::string& getClassName() const; //!< Get name of this class

	~InternalBuffer();

	//! Create internal buffer.
	/*!
		\return Internal buffer.
	*/
	static Ptr create();

	//! Get size.
	/*!
		\return Size of buffer in bytes.
	*/
	size_t getSize() const;

	//! Resize buffer to new size.
	/*!
		\param size New size of buffer in bytes.
	*/
	void resize(size_t size);
};

UNISHADER_END

#endif
