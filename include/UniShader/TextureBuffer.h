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
#ifndef TEXTURE_BUFFER_H
#define TEXTURE_BUFFER_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/Signal.h>
#include <UniShader/TextureUnit.h>
#include <UniShader/GLSLType.h>

#include <memory>
#include <string>
#include <deque>

UNISHADER_BEGIN

class BufferBase;
template <typename T> class Buffer;

//! Texture buffer class.
/*!
	Texture buffer is special type of texture that has its data
	stored in form of graphics card buffer. Texure buffer is therefore
	one-dimensional array of data.
	
	Advantage of texture buffers over basic one-dimensional textures lies
	in much bigger capacity.

	In GLSL code it is associated with samplerBuffer uniform.
*/

class UniShader_API TextureBuffer : public SignalReceiver, public ObjectBase{
private:
	TextureBuffer();
public:
	typedef std::shared_ptr<TextureBuffer> Ptr; //!< Shared pointer
	typedef std::shared_ptr<const TextureBuffer> PtrConst; //!< Shared pointer
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~TextureBuffer();

	//! Data type.
	/*!
		Data type is used to specify how data in buffer should be accessed.
	*/
	class DataType{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					CHAR, //!< Access data as chars.
					UNSIGNED_CHAR, //!< Access data as unsigned chars.
					SHORT, //!< Access data as short integers.
					UNSIGNED_SHORT, //!< Access data as unsigned short integers.
					HALF_FLOAT, //!< Access data as half precision floats (binary16).
					INT, //!< Access data as integers.
					UNSIGNED_INT, //!< Access data as unsigned integers.
					FLOAT //!< Access data as single precision floats.
		};
	private:
		myEnum m_en;
	public:
		DataType(){}
		DataType(const DataType& ref):m_en(ref.m_en){}
		DataType(myEnum en){ m_en = en; }
		DataType& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};


	//! Create texture buffer.
	/*!
		/return Texture buffer.
	*/
	static Ptr create();

	//! Connect buffer to texture buffer and set it as data source.
	/*!
		\param buffer Buffer.
		\param componentsNumber Number of components (elements) packed into single pixel.
		\param dataType Data type.
		\sa disconnectBuffer().
	*/
	void connectBuffer(std::shared_ptr<BufferBase> buffer, unsigned char componentsNumber, DataType dataType);

	//! Disconnect buffer from texture buffer.
	/*!
		\sa connectBuffer()
	*/
	void disconnectBuffer();

	//! Get components number.
	/*!
		\return Number of components packed into single pixel.
	*/
	unsigned char getComponentsNumber() const;

	//! Get data type.
	/*!
		\return Data type used to access data.
	*/
	DataType getDataType() const;

	//! Get texture unit index.
	/*!
		Texture unit index is availible only if texture buffer is active.
		\return Texture unit index.
	*/
	char getTextureUnitIndex() const;

	//! Set components number.
	/*!
		\param componentsNumber Number of components packed into single pixel.
	*/
	void setComponentsNumber(unsigned char componentsNumber);

	//! Set data type.
	/*!
		\param dataType Data type used to access data.
	*/
	void setDataType(DataType dataType);

	//! Prepare.
	/*!
		Prepare texture buffer for use.
		\return True if prepared successfully.
	*/
	bool prepare();

	//! Activate.
	/*!
		Activate texture buffer by locking a texture unit and modyfying its state.
		It is possible to activate texture buffer multiple times.
	*/
	void activate();

	//! Deactivate.
	/*!
		Deactivate texture by releasing texture unit.
		Texture unit is released only if all Activate() calls were matched by Deactivate().
	*/
	void deactivate();

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
	TextureUnit m_unit;
	std::shared_ptr<BufferBase> m_buffer;
	DataType m_dataType;
	unsigned int m_texture;
	unsigned int m_activeCount;
	unsigned char m_componentsNumber;
	bool m_prepared;
};

UNISHADER_END

#endif
