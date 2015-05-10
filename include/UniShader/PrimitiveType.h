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
#ifndef PRIMITIVE_TYPE_H
#define PRIMITIVE_TYPE_H

#include <UniShader/Utility.h>

UNISHADER_BEGIN

//! Primitive type that is allowed for rendering.
class PrimitiveType{
public:
	enum myEnum{NONE, //!< Uninitialized state.
				POINTS, //!< Points ( N primitives == N elements ).
				LINES, //!< Lines ( N primitives == N*2 elements ).
				LINE_STRIP, //!< Line strip ( N primitives == N+1 elements ).
                LINE_STRIP_ADJACENCY, //!< Line strip ( N primitives == N+3 elements ).
				TRIANGLES, //!< Triangles ( N primitives == N*3 elements ).
                TRIANGLE_STRIP, //!< Triangle strip ( N primitives == N+2 elements ).
	};
private:
	myEnum m_en;
public:
	PrimitiveType(){}
	PrimitiveType(const PrimitiveType& ref):m_en(ref.m_en){}
	PrimitiveType(myEnum en){ m_en = en; }
	PrimitiveType& operator =(myEnum en){ m_en = en; return *this; }
	operator myEnum(){ return m_en; }
};

UNISHADER_END

#endif
