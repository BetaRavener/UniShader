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
#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/SafePtr.h>
#include <UniShader/Signal.h>
#include <UniShader/GLSLType.h>

#include <memory>
#include <string>

UNISHADER_BEGIN

class ShaderProgram;
class BufferBase;
template <typename T> class Buffer;

//!	Attribute class.
/*!
	Attributes are input variables for GLSL shader programs.
	In newer GLSL versions attribute keyword was replaced by input keyword, but functionality
	is the same. Attributes are used for passing data that are different in each shader run.
	
	Data can be stored either in system memory or in buffers in graphics card memory. Using
	buffers is generally much faster than accessing system memory.
*/

class UniShader_API Attribute : public SignalSender, public SignalReceiver, public ObjectBase{
public:
	Attribute(ShaderProgram& program, std::string name);
	typedef SafePtr<Attribute> Ptr; //!< Safe pointer.
	typedef SafePtr<const Attribute> PtrConst; //!< Safe pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~Attribute();

	//!	Reading mode.
	/*!
		Reading modes are used to match method of reading data with actual data format in memory.
	*/
	class ReadingMode{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					CHAR, //!< Read memory as chars.
					UNSIGNED_CHAR, //!< Read memory as unsigned chars.
					SHORT, //!< Read memory as short integers.
					UNSIGNED_SHORT, //!< Read memory as unsigned short integers.
					INT, //!< Read memory as integers.
					UNSIGNED_INT, //!< Read memory as unsigned integers.
					FLOAT, //!< Read memory as single precision floats.
					DOUBLE //!< Read memory as double precision floats.
		};
	private:
		myEnum m_en;
	public:
		ReadingMode(){}
		ReadingMode(const ReadingMode& ref):m_en(ref.m_en){}
		ReadingMode(myEnum en){ m_en = en; }
		ReadingMode& operator =(myEnum en){ m_en = en; return *this; }
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

	//! Connect buffer to attribute and set it as data source.
	/*!
		\param buffer Buffer.
		\param offset Offset of first index.
		\param stride Stride between used indices.
		\sa disconnectBuffer().
	*/
	void connectBuffer(std::shared_ptr<BufferBase> buffer, unsigned int offset = 0, unsigned int stride = 0);

	//! Disconnect buffer from attribute.
	/*!
		\sa connectBuffer()
	*/
	void disconnectBuffer();

	//! Normalize integers to range <0; 1> .
	/*!
		Ignored if reading mode isn't integer mode.
		\param norm Normalize integers.
	*/
	void normalize(bool norm);

	//! Get buffer offset.
	/*!
		\return Buffer offset.
	*/
	unsigned int getBufferOffset() const;

	//! Get buffer stride.
	/*!
		\return Buffer stride.
	*/
	unsigned int getBufferStride() const;

	//! Get GLSL type.
	/*!
		\return GLSL type.
	*/
	const GLSLType& getGLSLType() const;

	//! Get reading mode.
	/*!
		\return reading mode.
	*/
	ReadingMode getReadingMode() const;

	//! Get shader variable name.
	/*!
		\return Shader variable name.
	*/
	std::string getName() const;
	
	//! Set buffer offset.
	/*!
		\param offset Offset of first index.
	*/
	void setBufferOffset(unsigned int offset);

	//! Set buffer stride.
	/*!
		\param stride Stride between used indices.
	*/
	void setBufferStride(unsigned int stride);

	//! Set reading mode.
	/*!
		\param readingMode Reading mode.
	*/
	void setReadingMode(ReadingMode readingMode);
	
	//! Prepare attribute.
	/*!
		Retrieve info about attribute from shader program and prepare attribute for use.
		\return True if prepared successfully.
	*/
	bool prepare();

	//! Apply attribute settings.
	/*!
		Modify OpenGL context with settings stored in this class.
	*/
	void apply();

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
	ShaderProgram& m_program;
	std::shared_ptr<BufferBase> m_buffer;
	GLSLType m_type;
	std::string m_name;
	int m_location;
	unsigned int m_offset;
	unsigned int m_stride;
	ReadingMode m_readingMode;
	bool m_normalize;
	bool m_prepared;
};

UNISHADER_END

#endif
