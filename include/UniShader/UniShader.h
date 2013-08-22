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
#ifndef UNISHADER_H
#define UNISHADER_H

//Include whole API
#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ShaderObject.h>
#include <UniShader/ShaderProgram.h>
#include <UniShader/ShaderInput.h>
#include <UniShader/ShaderOutput.h>
#include <UniShader/Buffer.h>
#include <UniShader/Attribute.h>
#include <UniShader/Uniform.h>
#include <UniShader/Varying.h>
#include <UniShader/TextureBuffer.h>
#include <UniShader/PrimitiveType.h>

#include <memory>
#include <string>

UNISHADER_BEGIN

//! UniShader class.
/*!
	UniShader is renderer class that utilizes OpenGL and ShaderProgram to
	render geometry.
*/

class UniShader_API UniShader{
public:
	UniShader();
	~UniShader();

	//! Connect program.
	/*!
		\param program Shader program used for rendering.
	*/
	void connectProgram(std::shared_ptr<ShaderProgram>& program);

	//! Gets currently used program.
	/*!
		\return Program used by UniShader.
	*/
	std::shared_ptr<ShaderProgram> program();

	//! Disconnect program.
	void disconnectProgram();

	//! Render.
	/*!
		Render array with OpenGL glDrawArrays command using ShaderProgram.
		\param primitiveType Primitive type that will be rendered.
		\param primitiveCount Number of primitives that will be rendered.
		\param offset Global offset for all inputs.
		\param record If true, shader output will be recorded and stored
		\param wait If true, function won't return until all OpenGL commands haven't been processed.
	*/
	void render(PrimitiveType primitiveType, unsigned int primitiveCount, unsigned int offset = 0, bool record = true, bool wait = false);

	//! Render elements.
	/*!
		Render elements with OpenGL glDrawRangeElements command using ShaderProgram.
		\param elementsBuffer Buffer of indices to be rendered.
		\param primitiveType Primitive type that will be rendered.
		\param primitiveCount Number of primitives that will be rendered.
		\param offset Global offset for all inputs.
		\param record If true, shader output will be recorded and stored
		\param wait If true, function won't return until all OpenGL commands haven't been processed.
	*/
	void renderElements(Buffer<unsigned int>::Ptr elementsBuffer, PrimitiveType primitiveType, unsigned int primitiveCount, unsigned int offset = 0, bool record = true, bool wait = false);

private:
	ShaderProgram::Ptr m_program;
};

UNISHADER_END

#endif
