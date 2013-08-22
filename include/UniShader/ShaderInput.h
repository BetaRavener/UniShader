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
#ifndef SHADER_INPUT_H
#define SHADER_INPUT_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/SafePtr.h>
#include <UniShader/Signal.h>

#include <deque>

UNISHADER_BEGIN

class ShaderProgram;
class Attribute;
class Uniform;

//! Shader input class.
/*!
	Shader input manages all input operations for shader program.
*/

class UniShader_API ShaderInput : public SignalReceiver, public ObjectBase{
public:
	ShaderInput(ShaderProgram& program);
	typedef SafePtr<ShaderInput> Ptr; //!< Safe pointer
	typedef SafePtr<const ShaderInput> PtrConst; //!< Safe pointer
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~ShaderInput();

	//! Signal identifier enum.
	/*!
		Describe signal sent from this class.
	*/
	class SignalID{
	public:
		enum Types{CHANGED //!< Class changed its state.
		}; 
	};

	//! Add new attribute.
	/*!
		Create and add new attribute to shader input.
		If attribute with same name already exists, pointer to that attribute is returned.
		\param name Name of attribute.
		\return Pointer to attribute.
	*/
	SafePtr<Attribute> addAttribute(std::string name);

	//! Add new uniform.
	/*!
		Create and add new uniform to shader input.
		If uniform with same name already exists, pointer to that uniform is returned.
		\param name Name of uniform.
		\return Pointer to uniform.
	*/
	SafePtr<Uniform> addUniform(std::string name);

	//! Get attribute.
	/*!
		Return pointer to previously added attribute.
		If attribute with the name doesn't exists, null pointer is returned.
		\param name Name of attribute.
		\return Pointer to attribute.
	*/
	SafePtr<Attribute> getAttribute(std::string name);

	//! Get uniform.
	/*!
		Return pointer to previously added uniform.
		If uniform with the name doesn't exists, null pointer is returned.
		\param name Name of uniform.
		\return Pointer to uniform.
	*/
	SafePtr<Uniform> getUniform(std::string name);

	//! Remove attribute.
	/*!
		Destroy attribute and remove it from shader input.
		If attribute with the name doesn't exists, function returns silently.
		\param name Name of attribute.
	*/
	void removeAttribute(std::string name);

	//! Remove uniform.
	/*!
		Destroy uniform and remove it from shader input.
		If uniform with the name doesn't exists, function returns silently.
		\param name Name of uniform.
	*/
	void removeUniform(std::string name);

	//! Prepare.
	/*!
		Prepare input and underlying classes for use.
		\return True if prepared successfully.
	*/
	void prepare();

	//! Activate.
	/*! 
		Activate input by modyfying OpenGL context and make it availible for shader program.
	*/
	void activate();

	//! Is active?
	/*!
		Return if shader input is active.
		\return True if active.
	*/
	bool isActive();

	//! Deactivate.
	/*!
		Return OpenGL context states modified by this class to their default state.
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
	ShaderProgram& m_program;
	std::deque< std::shared_ptr<Attribute> > m_attribs;
	std::deque< std::shared_ptr<Uniform> > m_uniforms;
	unsigned int m_VAO;
	bool m_remakeVAO;
	bool m_active;
};

UNISHADER_END

#endif
