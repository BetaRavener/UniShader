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

#pragma once
#ifndef BUFFER_H
#define BUFFER_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>

#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>

UNISHADER_BEGIN

//!	Buffer base class.
/*!
	Buffer base stores raw OpenGL information about buffer object. 
*/

class UniShader_API BufferBase : public ObjectBase{
protected:
	BufferBase();
public:
	typedef std::shared_ptr<BufferBase> Ptr; //!< Shared pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~BufferBase();

	//! Frequency mode.
	/*!
		Hint for OpenGL on how to store data in graphics card memory for optimal access time.
	*/
	class FrequencyMode{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					STREAM, //!< The data store contents will be modified once and used at most a few times.
					STATIC, //!< The data store contents will be modified once and used many times.
					DYNAMIC //!< The data store contents will be modified repeatedly and used many times.
		};
	private:
		myEnum m_en;
	public:
		FrequencyMode(){}
		FrequencyMode(const FrequencyMode& ref):m_en(ref.m_en){}
		FrequencyMode(myEnum en){ m_en = en; }
		FrequencyMode& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};

	//! Nature mode.
	/*!
		Hint for OpenGL on how to store data in graphics card memory for optimal access time.
	*/
	class NatureMode{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					DRAW, //!< The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
					READ, //!< The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
					COPY //!< The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
		};
	private:
		myEnum m_en;
	public:
		NatureMode(){}
		NatureMode(const NatureMode& ref):m_en(ref.m_en){}
		NatureMode(myEnum en){ m_en = en; }
		NatureMode& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};

	//! Signal identifier enum.
	/*!
		Describe signal sent from this class.
	*/
	class SignalID{
	public:
		enum Types{CHANGED //!< Class changed its state.
		}; 
	};

	//! Clear data.
	/*!
		Clear all buffer data leaving buffer empty with zero size.
		\return True if cleared successfully.
	*/
	bool clear();

	//! Get OpenGL buffer identifier.
	/*! 
		\return Numeric identifier of buffer object in OpenGL.
	*/
	unsigned int getGlID() const;

	//! Get frequency mode.
	/*! 
		\return Frequency mode.
	*/
	FrequencyMode getFrequencyMode() const;

	//! Get nature mode.
	/*! 
		\return Nature mode.
	*/
	NatureMode getNatureMode() const;

	//! Set frequency mode.
	/*! 
		New mode will be applied after next setData() call.
		\param frequencyMode Frequency mode.
	*/
	void setFrequencyMode(FrequencyMode frequencyMode);
	
	//! Set nature mode.
	/*! 
		New mode will be applied in next setData() call.
		\param natureMode Nature mode.
	*/
	void setNatureMode(NatureMode natureMode);

protected:
	//! Map buffer from graphics card memory to system memory.
	/*!
		\param mappedPtr Pointer to pointer to first index.
	*/
	bool mapBuffer(void** mappedPtr) const;

	//! Unmap buffer from system memory
	bool unmapBuffer() const;

	//! Set buffer with plain data.
	/*!
		\param data Pointer to plain data.
		\param size Size of plain data in bytes.
		\return True if data were set successfully.
	*/
	bool setPlainData(const void* data, size_t size);

	size_t m_byteSize;
private:
	FrequencyMode m_frequencyMode;
	NatureMode m_natureMode;
	unsigned int m_bufferID;
};

//! Buffer class.
/*!
	Templated buffer class is interface for manipulating buffer objects.
	It is allowed to use buffer only for input or output at a time. Using
	buffer as both input and output at the same time can result in undefined behavior.
*/

template <typename T>
class Buffer : public BufferBase{
private:
	Buffer();
public:
	typedef std::shared_ptr<Buffer<T>> Ptr; //!< Shared pointer
	typedef std::shared_ptr<const Buffer<T>> PtrConst; //!< Shared pointer
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~Buffer();

	//! Create buffer.
	/*!
		\return Buffer.
	*/
	static Ptr create();

	//! Get data copy.
	/*!
		Copies buffer data from graphics card memory to vector.
		\param vec Vector storage for data. Passed vector must be empty.
		\return True if data were copied successfully.
	*/
	bool getDataCopy(std::vector<T>& vec) const;

	//! Get data copy.
        /*!
		Copies buffer data from graphics card memory to array.
		\param arr Pointer to data. Passed pointer must be NULL.
		\param size Size of array in elements.
		\return True if data were copied successfully.
	*/
	bool getDataCopy(T* arr, unsigned int& size) const;

	//! Get string.
	/*! 
		Convert values to string. Values are delimited by space.
		\param begin First index.
		\param count Number of elements to convert.
		\param format Format used for converting values into string.
		\return Nonempty string if data were retrieved successfully and buffer is not zero sized.
	*/
	std::string getString(unsigned int begin = 0, unsigned int count = 0, const std::ios& format = std::ios(new std::stringbuf())) const;

	//! Set data.
	/*!
		\param vec Vector with data.
		\return True if data were set successfully.
	*/
	bool setData(const std::vector<T>& vec);

	//! Set data.
	/*!
		\param arr Array with data.
		\param size Size of array in elements.
		\return True if data were set successfully.
	*/
	bool setData(const T* arr, unsigned int size);
};

UNISHADER_END

#include <UniShader/Buffer.inl>

#endif
