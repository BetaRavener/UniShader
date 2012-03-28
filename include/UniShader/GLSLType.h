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
#ifndef GLSL_TYPE_H
#define GLSL_TYPE_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>

UNISHADER_BEGIN

//! GLSL type info class.
/*!
	GLSL type class stores all the information about variable in shader program.
	It is automatically filled by interface class and can be retrieved after successfull linkage.
*/

class UniShader_API GLSLType{
public:
	GLSLType();
	GLSLType(const GLSLType& ref);
	
	//! Object type.
	class ObjectType{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					VALUE, //!< Value object (basic variable)
					SAMPLER, //!< Sampler
					IMAGE //!< Image
		};
	private:
		myEnum m_en;
	public:
		ObjectType(){}
		ObjectType(const ObjectType& ref):m_en(ref.m_en){}
		ObjectType(myEnum en){ m_en = en; }
		ObjectType& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};

	//! Data type.
	class DataType{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					INT, //!< Integer variable.
					UNSIGNED_INT, //!< Unsigned integer variable.
					FLOAT, //!< Single precision float variable.
					DOUBLE //!< Double precision float variable.
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

	//! Sampler type.
	class SamplerType{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					ONE_DIMENSIONAL, //!< One dimensional sampler.
					TWO_DIMENSIONAL, //!< Two dimensional sampler.
					THREE_DIMENSIONAL, //!< Three dimensional sampler.
					CUBE_MAPPED, //!< Cube mapped sampler.
					BUFFER //!< Sampler buffer.
		};
	private:
		myEnum m_en;
	public:
		SamplerType(){}
		SamplerType(const SamplerType& ref):m_en(ref.m_en){}
		SamplerType(myEnum en){ m_en = en; }
		SamplerType& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};

	ObjectType getObjectType() const;
	DataType getDataType() const;
	unsigned char getColumnSize() const;
	unsigned char getColumnCount() const;
	SamplerType getSamplerType() const;

private:
	friend class TypeResolver;

	void clear();

	ObjectType m_objectType;
	DataType m_dataType;
	unsigned char m_columnSize;
	unsigned char m_columnCount;
	SamplerType m_samplerType;
};

UNISHADER_END

#endif
