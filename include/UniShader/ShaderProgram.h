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
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/SafePtr.h>
#include <UniShader/Signal.h>
#include <UniShader/PrimitiveType.h>

#include <memory>
#include <deque>

UNISHADER_BEGIN

class UniShader;
class ShaderObject;
class ShaderInput;
class ShaderOutput;

//! Shader program class.
/*!
	Shader program is set of instructions that are executed on graphics card hardware during
	programmable pipeline pass. Programmable pipeline consists of 3 stages that are executed 
	in following order: Vertex, Geometry, Fragment. 

	Valid output is required for a single stage to be valid. Output is valid if it is passed
	from previous to next stage and eventually used for computing final pixel color output in
	fragment stage or if it is recorded.
	
	In vertex stage, vertices are processed one at a time.
	In geometry stage, primitives are processed one at a time.
	In fragment stage, final pixels are processed one at a time.

	For further information see http://www.opengl.org/sdk/docs/tutorials/TyphoonLabs/Chapter_1.pdf
*/

class UniShader_API ShaderProgram : public SignalSender, public SignalReceiver, public ObjectBase{
private:
	ShaderProgram();
public:
	typedef std::shared_ptr<ShaderProgram> Ptr; //!< Shared pointer.
	typedef std::shared_ptr<const ShaderProgram> PtrConst; //!< Shared pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~ShaderProgram();

	//! Program ink status.
	class LinkStatus{
	public:
		enum myEnum{NONE, //!< Uninitialized state.
					PENDING_LINK, //!< Program need relink.
					SUCCESSFUL_LINK, //!< Program was successfully linked.
					FAILED_LINK //!< Program failed to link.
		};
	private:
		myEnum m_en;
	public:
		LinkStatus(){}
		LinkStatus(const LinkStatus& ref):m_en(ref.m_en){}
		LinkStatus(myEnum en){ m_en = en; }
		LinkStatus& operator =(myEnum en){ m_en = en; return *this; }
		operator myEnum(){ return m_en; }
	};
	
	//! Signal identifier enum.
	/*!
		Describe signal sent from this class.
	*/
	class SignalID{
	public:
		enum Types{RELINKED //!< Program was relinked.
		}; 
	};


	//! Create shader program.
	/*!
		\return Shader program.
	*/
	static Ptr create(); 

	//! Add shader object.
	/*!
		Add new shader object to shader program.
		If shader object is already part of shader program, function returns silently.
		\param shaderObjPtr Pointer to shader object.
	*/
	void addShaderObject(std::shared_ptr<ShaderObject>& shaderObjPtr);

	//! Remove shader object.
	/*!
		Remove new shader object to shader program.
		If shader object isn't part of shader program, function returns silently.
		\param shaderObjPtr Pointer to shader object.
	*/
	bool removeShaderObject(std::shared_ptr<ShaderObject>& shaderObjPtr);

	//! Get shader input.
	/*!
		Return pointer to shader input associated with this shader program.
		\return Pointer to shader input.
	*/
	SafePtr<ShaderInput> getInput();

	//! Get shader output.
	/*!
		Return pointer to shader output associated with this shader program.
		\return Pointer to shader output.
	*/
	SafePtr<ShaderOutput> getOutput();

	//! Get OpenGL shader program identifier.
	/*! 
		\return Numeric identifier of shader program in OpenGL.
	*/
	unsigned int getGlID() const;

	//! Get program link status.
	/*!
		\return Link status.
	*/
	LinkStatus getLinkStatus() const;

	//! Ensure linkage after performing changes to program 
	/*!
		\return True if resulting link status is LinkStatus::SUCCESSFUL_LINK
	*/
	bool ensureLink();

	//! Activate without recording.
	/*! 
		Activate program by modifying OpenGL context. Also prepare underlying classes for use.
		\return True if program was activated successfully.
	*/
	bool activate();

	//! Activate with recording.
	/*! 
		Activate program by modifying OpenGL context. Also prepare underlying classes for use.
		\param primitiveType Primitive type that will be recorded.
		\param primitiveCount Number of primitives that will be recorded.
		\return True if program was activated successfully.
	*/
	bool activate(PrimitiveType primitiveType, unsigned int primitiveCount);

	//! Is active?
	/*!
		Return if program is active.
		\return True if active.
	*/
	bool isActive();

	//! Deactivate.
	/*!
		Return OpenGL context states modified by this class to their default state.
	*/
	bool deactivate();

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
	bool link();
	int printProgramInfoLog() const;
   
	std::shared_ptr<ShaderInput> m_input;
	std::shared_ptr<ShaderOutput> m_output;
	std::deque<std::shared_ptr<ShaderObject>> m_shaderObjects;
	unsigned int m_programObjectID;
	LinkStatus m_linkStatus;
	bool m_active;
};

UNISHADER_END

#endif
