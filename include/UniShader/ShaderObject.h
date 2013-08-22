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
#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/Signal.h>

#include <memory>
#include <string>

UNISHADER_BEGIN

//! Shader object class.
/*!
	Shader objects are elementary parts of shader program. They are
	similar to C++ object files as they are each compiled separately
	and then can be mixed to create complex programs.

	There are vertex, geometry and fragment shader objects, each affecting
	corresponding stage in shader pipeline. By default, files with extension
	*.vert are recognized as vertex, *.geom as geometry and .frag as fragment objects.
*/

class UniShader_API ShaderObject : public SignalSender, public ObjectBase{
private:
	ShaderObject();

public:
	typedef std::shared_ptr<ShaderObject> Ptr; //!< Shared pointer.
	typedef std::shared_ptr<const ShaderObject> PtrConst; //!< Shared pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~ShaderObject();

	//! Shader object type.
	class Type{
	public:
		enum myEnum{NONE, //!< Uninitialized state - automatic recognition.
					VERTEX, //!< Vertex shader object.
					GEOMETRY, //!< Geometry shader object.
					FRAGMENT, //!< Fragment shader object.
					UNRECOGNIZED //!< Unrecognized shader object - automatic recognition failed.
		};
	private:
		myEnum m_en;
	public:
		Type(){}
		Type(const Type& ref):m_en(ref.m_en){}
		Type(myEnum en){ m_en = en; }
		Type& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};

	//! Shader object compilation status.
	class CompilationStatus{
	public:
		enum myEnum{PENDING_COMPILATION, //!< Shader object need recompilation.
					SUCCESSFUL_COMPILATION, //!< Shader object was compiled successfully.
					FAILED_COMPILATION //!< Shader object compilation failed.
		};
	private:
		myEnum m_en;
	public:
		CompilationStatus(){}
		CompilationStatus(const CompilationStatus& ref):m_en(ref.m_en){}
		CompilationStatus(myEnum en){ m_en = en; }
		CompilationStatus& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};
	
	//! Signal identifier enum.
	/*!
		Describe signal sent from this class.
	*/
	class SignalID{
	public:
		enum Types{	CHANGED, //!< Class changed its state.
					RECOMPILED //!< Shader object was recompiled.
		}; 
	};

	//! Create shader object.
	/*!
		\return Shader object.
	*/
	static Ptr create();

	//! Load source code for shader object from file.
	/*!
		\param fileName Name of file with source code.
		\param shaderType Type of shader object.
		\return True if loaded successfully
	*/
	bool loadFile(const std::string fileName, Type shaderType = Type::NONE);

	//! Load source code for shader object from string.
	/*!
		\param code Source code.
		\param shaderType Type of shader object.
		\return True if loaded successfully
	*/
	bool loadCode(const std::string code, Type shaderType);

	//! Ensure compilation.
	/*
		Compile shader object if needed.
		\return True if shader object is successfully compiled.
	*/
	bool ensureCompilation();
	
	//! Get OpenGL shader object identifier.
	/*! 
		\return Numeric identifier of shader object in OpenGL.
	*/
	unsigned int getGlID() const;

	//! Get shader object type.
	/*!
		\return Type of shader object.
	*/
	Type getType() const;

	//! Get shader object compilation status.
	/*!
		\return Compilation status.
	*/
	CompilationStatus getCompilationStatus() const;

private:
	bool compile();
	bool printShaderInfoLog() const;
	int getShaderSize(const std::string &shaderName) const;
	bool readShaderSource(const std::string& fileName, std::string& shaderText);
	bool translateLiterals(std::string &shaderText);

	unsigned int m_shaderObjectID;
	Type m_type;
	CompilationStatus m_compilationStatus;
};

UNISHADER_END

#endif
