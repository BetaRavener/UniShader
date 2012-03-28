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
				TRIANGLES, //!< Triangles ( N primitives == N*3 elements ).
				TRIANGLE_STRIP //!< Triangle strip ( N primitives == N+2 elements ).
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
