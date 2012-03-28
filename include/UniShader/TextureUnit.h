/*
* UniShader - Interface for GPGPU and working with shader programs
* Copyright (c) 2011-2012 Ivan Sevcik - ivan-sevcik@hotmail.com
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
#ifndef TEXTURE_UNIT_H
#define TEXTURE_UNIT_CONTROLLER_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>

#include <deque>

UNISHADER_BEGIN

//! Texture unit class.
/*! 
	Texture unit is a part of graphics card architecture. It is a 
	component generally used for mapping texture on 3D geometry. 
	Each graphics card have at least two of texture units.

	Because only one texture can be associated with texture unit at
	a time, texture units have to be managed.
*/

class TextureUnit{
public:
	TextureUnit();
	~TextureUnit();

	//! Lock.
	/*!
		Lock texture unit, making it unavailible.
	*/
	void lock();

	//! Make texture unit active.
	/*!
		OpenGL texture modifying calls affect texture associated with active texture unit.
		Only one texture unit can be active in OpenGL context at a time.
		Texture unit must be locked before activating, otherwise error is produced.
		\return True if texture unit was made active susccessfully.
	*/
	bool makeActive();

	//! Get texture unit index.
	/*!
		Return index of texture unit.
		Availible texture units in OpenGL are enumerated as GL_TEXTURE0, GL_TEXTURE1,..
		\return Texture unit index.
	*/
	char getIndex() const;

	//! Release.
	/*!
		Release texture unit, making it availible.
	*/
	void release();
private:
	static std::deque<char> m_freeTextureUnits;
	static bool m_initialized;

	char m_index;
	bool m_locked;
};

UNISHADER_END

#endif
