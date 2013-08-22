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
#ifndef VARYING_H
#define VARYING_H

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
class ShaderOutput;
class BufferBase;
template <typename T> class Buffer;

//! Varying class.
/*!
	Varyings are output variables for GLSL vertex and geometry shader programs.
	In newer GLSL versions varying keyword was replaced by output keyword, but functionality
	is the same. Varyings are used to pass data between subsequent shader stages.

	It is possible to record values in varying before they enter fragment shader stage with
	transform feedback. This class, therefore, seves as interface to access recorded values
	in application. Recorded values are originaly stored in buffer in graphics card memory.

	If a geometry shader stage is present, only output variables of geometry shader can be recorded.
	If only vertex shader stage is present, recording of output variable is considered as valid
	usage of that variable and shader program can be run.
*/

class UniShader_API Varying : public SignalReceiver, public ObjectBase{
public:
	Varying(ShaderProgram& program, ShaderOutput& output, std::string name);
	typedef SafePtr<Varying> Ptr; //!< Safe pointer.
	typedef SafePtr<const Varying> PtrConst; //!< Safe pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~Varying();

	//! Get buffer.
	/*!
		\return Buffer.
	*/
	template <typename T> typename std::shared_ptr<const Buffer<T>> getBuffer();

	//! Get GLSL type.
	/*!
		\return GLSL type.
	*/
	const GLSLType& getGLSLType() const;

	//! Get shader variable name.
	/*!
		\return Shader variable name.
	*/
	std::string getName() const;
	
	//! Prepare varying.
	/*!
		Retrieve info about varying from shader program and prepare varying for use.
		\return True if prepared successfully.
	*/
	bool prepare(unsigned int index, unsigned int primitiveCount, size_t* unitSize = 0);

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
    
	ShaderProgram& m_program;
	ShaderOutput& m_output;
	GLSLType m_glslType;
	std::string m_name;
	std::shared_ptr<BufferBase> m_buffer;
	size_t m_unitSize;
	bool m_prepared;
};

UNISHADER_END
        
#include <UniShader/Varying.inl>

#endif
