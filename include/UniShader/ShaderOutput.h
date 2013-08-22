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
#ifndef SHADER_OUTPUT_H
#define SHADER_OUTPUT_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/Signal.h>
#include <UniShader/SafePtr.h>
#include <UniShader/PrimitiveType.h>

#include <memory>
#include <vector>
#include <deque>

UNISHADER_BEGIN

class ShaderProgram;
class Varying;
class BufferBase;
template <typename T> class Buffer;

//! Shader output class.
/*!
	Shader output manages all output operations for shader program.
*/

class UniShader_API ShaderOutput : public SignalSender, public ObjectBase{
public:
	ShaderOutput(ShaderProgram& program);
	typedef SafePtr<ShaderOutput> Ptr; //!< Safe pointer
	typedef SafePtr<const ShaderOutput> PtrConst; //!< Safe pointer
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~ShaderOutput();

	//! Signal identifier enum.
	/*!
		Describe signal sent from this class.
	*/
	class SignalID{
	public:
		enum Types{	CHANGED, //!< Class changed its state.
					INTERLEAVED, //!< Output was set to interleaved mode
					DEINTERLEAVED //!< Output was set to deinterleaved mode
		}; 
	};

	//! Add new varying.
	/*!
		Create and add new varying to shader output.
		If varying with same name already exists, pointer to that varying is returned.
		\param name Name of varying.
		\return Pointer to varying.
	*/
	SafePtr<Varying> addVarying(std::string name);

	//! Get varying.
	/*!
		Return pointer to previously added varying.
		If varying with the name doesn't exists, null pointer is returned.
		\param name Name of varying.
		\return Pointer to varying.
	*/
	SafePtr<Varying> getVarying(std::string name);

	//! Remove varying.
	/*!
		Destroy varying and remove it from shader output.
		If varying with the name doesn't exists, function returns silently.
		\param name Name of varying.
	*/
	void removeVarying(std::string name);

	//! Interleave shader output.
	/*!
		If shader output is interleaved, all values are recorded to single buffer.
		In interleaved mode all underlying varyings return this buffer. 
		In deinterleaved (separate) mode each varying has it own buffer and values are split between them.
	*/
	void interleave(bool interl);

	//! Is interleaved?
	/*!
		Returns if output is interleaved.
		\return True if interleaved.
	*/
	bool isInterleaved();

	//! Get interleaved buffer.
	/*!
		Return pointer to interleaved buffer.
		If output is not interlraved, null pointer is returned and error is generated.
		\return Buffer.
	*/
	template <typename T> typename std::shared_ptr<const Buffer<T>> getInterleavedBuffer();

	//! Set up.
	/*!
		Set up underlying varyings to be recorded.
		This will affect next shader program linkage.
	*/
	void setUp();

	//! Prepare.
	/*!
		Prepare output and underlying classes for use.
		\param primitiveCount Number of primitives that will be recorded.
		\return True if prepared successfully.
	*/
	bool prepare(unsigned int primitiveCount);

	//! Activate.
	/*! 
		Activate output by modyfying OpenGL context and make it availible for shader program.
	*/
	void activate(PrimitiveType primitiveType);

	//! Is active?
	/*!
		Return if shader output is active.
		\return True if active.
	*/
	bool isActive();

	//! Deactivate.
	/*!
		Return OpenGL context states modified by this class to their default state.
	*/
	void deactivate();
private:
	ShaderProgram& m_program;
	std::deque< std::shared_ptr<Varying> > m_varyings;
	std::vector<const char*> m_names;
	std::shared_ptr<BufferBase> m_interleavedBuffer;
	size_t m_overallSize;
	unsigned int m_query;
	int m_primitivesProcessed;
	bool m_interleaved;
	bool m_prepared;
	bool m_active;
};

UNISHADER_END

#include <UniShader/ShaderOutput.inl>

#endif
