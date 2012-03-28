#pragma once
#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include <UniShader/Utility.h>

//	Required by all UniShader objects for returning class name
#include <string>

//!	Object base class.
/*!
	All UniShader interface objects must have a function that
	returns their name and a hidden constructor.
*/

UNISHADER_BEGIN
class UniShader_API ObjectBase{
protected:

	//! Hidden class constructor.
	ObjectBase(){}

public:

	//! Pure virtual function returning class name.
	virtual const std::string& getClassName() const = 0;

};
UNISHADER_END

#endif