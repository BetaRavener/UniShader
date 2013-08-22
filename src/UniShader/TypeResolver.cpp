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

#include <UniShader/TypeResolver.h>

//see http://www.opengl.org/wiki/GLAPI/glGetActiveUniform

using UNISHADER_NAMESPACE;

bool TypeResolver::resolve(GLenum typeEnum, GLSLType& type){
	type.clear();
	switch(typeEnum){
	case GL_FLOAT:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 1;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_VEC2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_VEC3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_VEC4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;

	case GL_DOUBLE:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 1;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_VEC2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_VEC3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_VEC4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;

	case GL_INT:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 1;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_VEC2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_VEC3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_VEC4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;

	case GL_UNSIGNED_INT:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 1;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_VEC2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_VEC3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_VEC4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 1;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;

	case GL_FLOAT_MAT2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT2x3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT2x4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT3x2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT3x4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT4x2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_FLOAT_MAT4x3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;

	case GL_DOUBLE_MAT2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT2x3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT2x4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 2;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT3x2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT3x4:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 4;
		type.m_columnCount = 3;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT4x2:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 2;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;
	case GL_DOUBLE_MAT4x3:
		type.m_objectType = GLSLType::ObjectType::VALUE;
		type.m_columnSize = 3;
		type.m_columnCount = 4;
		type.m_dataType = GLSLType::DataType::DOUBLE;
		return SUCCESS;

	case GL_SAMPLER_1D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::ONE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_SAMPLER_2D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::TWO_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_SAMPLER_3D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::THREE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_SAMPLER_CUBE:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::CUBE_MAPPED;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;
	case GL_SAMPLER_BUFFER:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::BUFFER;
		type.m_dataType = GLSLType::DataType::FLOAT;
		return SUCCESS;

	case GL_INT_SAMPLER_1D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::ONE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_SAMPLER_2D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::TWO_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_SAMPLER_3D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::THREE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_SAMPLER_CUBE:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::CUBE_MAPPED;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;
	case GL_INT_SAMPLER_BUFFER:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::BUFFER;
		type.m_dataType = GLSLType::DataType::INT;
		return SUCCESS;

	case GL_UNSIGNED_INT_SAMPLER_1D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::ONE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_SAMPLER_2D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::TWO_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_SAMPLER_3D:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::THREE_DIMENSIONAL;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_SAMPLER_CUBE:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::CUBE_MAPPED;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
	case GL_UNSIGNED_INT_SAMPLER_BUFFER:
		type.m_objectType = GLSLType::ObjectType::SAMPLER;
		type.m_samplerType = GLSLType::SamplerType::BUFFER;
		type.m_dataType = GLSLType::DataType::UNSIGNED_INT;
		return SUCCESS;
		
	default:
		std::cerr << "ERROR: Unknown GLSL data type" << std::endl;
		return FAILURE;
	}
}
