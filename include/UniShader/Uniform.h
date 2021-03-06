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
#ifndef UNIFORM_H
#define UNIFORM_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/SafePtr.h>
#include <UniShader/Signal.h>
#include <UniShader/GLSLType.h>

#include <memory>
#include <vector>

UNISHADER_BEGIN
	
class ShaderProgram;
class TextureBuffer;
class Texture;

//! Uniform class.
/*!
	Uniforms are constant objects passed to shader program from application.
	They can be either values, samplers or images and they are uniform throughout
	whole shader pipeline. This means, for example that uniform with same name in
	vertex stage and fragment stage must have the same type.

	Values (basic variables) are constant values.

	Samplers are objects referring to a texture or buffer. Special fetch functions
	are used to extract data from its data sources.

	Images ... aren't supported yet.

	NOTE: Uniforms are very complex, therefore only Values and Sampler buffers are
	currently supported. Also interface around uniforms might change in near future.
*/

class UniShader_API Uniform : public SignalReceiver, public ObjectBase{
public:
	Uniform(ShaderProgram& program, std::string name);
	typedef SafePtr<Uniform> Ptr; //!< Safe pointer.
	typedef SafePtr<const Uniform> PtrConst; //!< Safe pointer.
	virtual const std::string& getClassName() const; //!< Get name of this class.
	~Uniform();

	//! Get shader constant name.
	/*!
		\return Shader constant name.
	*/
	std::string getName() const;

	//! Transpose matrix.
	/*!
		If uniform is matrix type, transposing results in matrix mirrored across main diagonal.
		\param transpose If true, matrix is transposed.
	*/
	void transposeMatrix(bool transpose);

	//! Returns if matrix is transposed.
	/*!
		\return True if matrix is transposed.
	*/
	bool isMatrixTransposed();

	//! Clear source.
	/*!
		Clear any source of data, leaving uniform without source.
	*/
	void clearSource();

	//! Set source of data.
	/*!
		\param val Value to be used as data.
	*/
	void setSource(float val);

	//! Set source of data.
	/*!
		\param val Value to be used as data.
	*/
	void setSource(int val);

	//! Set source of data.
	/*!
		\param val Value to be used as data.
	*/
	void setSource(unsigned int val);

	//! Set source of data.
	/*!
		\param arr Array with data.
		\param size Size of array in elements.
	*/
	void setSource(const float* arr, unsigned int size);

	//! Set source of data.
	/*!
		\param arr Array with data.
		\param size Size of array in elements.
	*/
	void setSource(const int* arr, unsigned int size);

	//! Set source of data.
	/*!
		\param arr Array with data.
		\param size Size of array in elements.
	*/
	void setSource(const unsigned int* arr, unsigned int size);

	//! Set source of data.
	/*!
		\param vec Vector with data.
	*/
	void setSource(const std::vector<float>& vec);

	//! Set source of data.
	/*!
		\param vec Vector with data.
	*/
	void setSource(const std::vector<int>& vec);

	//! Set source of data.
	/*!
		\param vec Vector with data.
	*/
	void setSource(const std::vector<unsigned int>& vec);

    //! Set source of data.
    /*!
        \param texture Texture with data.
    */
    void setSource(std::shared_ptr<Texture>& texture);

	//! Set source of data.
	/*!
		\param textureBuffer Texture buffer with data.
	*/
	void setSource(std::shared_ptr<TextureBuffer>& textureBuffer);

	//! Get GLSL type.
	/*!
		\return GLSL type.
	*/
	const GLSLType& getGLSLType() const;

	//! Prepare uniform.
	/*!
		Retrieve info about uniform from shader program and prepare uniform for use.
		\return True if prepared successfully.
	*/
	bool prepare();

	//! Apply uniform settings.
	/*!
		Modify OpenGL context with settings stored in this class.
	*/
	void apply();

	//! Deactivate active texture source.
	/*!
		Applying uniform with a texture source activates the texture.
		This function is used to deactivate it afterwards.
	*/
	void deactivateTextureSource();

	//! Handle incoming signal.
	/*!
		\param signalID Signal identifier.
		\param callerPtr Pointer to object sending signal.
		\return True if handled.
	*/
	virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
	ShaderProgram& m_program;
	GLSLType m_type;
	std::string m_name;

    std::shared_ptr<Texture> m_texture;
	std::shared_ptr<TextureBuffer> m_textureBuffer;
	char* m_plainData;
	size_t m_dataByteSize;

	int m_location;
	bool m_transposeMatrix;
	bool m_prepared;
	bool m_applied;
};

UNISHADER_END

#endif
