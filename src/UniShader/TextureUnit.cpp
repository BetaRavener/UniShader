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

#include <UniShader/TextureUnit.h>
#include <UniShader/OpenGL.h>

#include <iostream>

using UNISHADER_NAMESPACE;

bool TextureUnit::m_initialized = false;
std::deque<char> TextureUnit::m_freeTextureUnits;

TextureUnit::TextureUnit():
m_index(-1),
m_locked(0){
	if(!m_initialized){
		ensureGlewInit();
		clearGLErrors();

		GLint max, mctiu;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &max);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &mctiu);
		if(max < mctiu)
			max = mctiu;
        if(max > 255)
            std::cerr << "More texture units should be allowed, report this to UniShader forums" << std::endl;
		for(int i = 0; i < max; i++)
			m_freeTextureUnits.push_back((char)i);

		m_initialized = true;
	}
}

TextureUnit::~TextureUnit(){
	if(m_locked)
		m_freeTextureUnits.push_front(m_index);
}

void TextureUnit::lock(){
	if(m_locked)
		return;

	if(m_freeTextureUnits.size() == 0){
		std::cerr << "ERROR: Number of active textures exceeds number of texture units" << std::endl;
		return;
	}
	else{
		m_index = m_freeTextureUnits.front();
		m_freeTextureUnits.pop_front();
		m_locked = true;
	}
}

bool TextureUnit::makeActive(){
	if(m_locked){
		ensureGlewInit();
		clearGLErrors();

		glActiveTexture(GL_TEXTURE0+m_index);
		return (!printGLError());
	}
	else{
		std::cerr << "ERROR: Texture unit must be locked before activating" << std::endl;
		return FAILURE;
	}
}

char TextureUnit::getIndex() const{
	return m_index;
}

void TextureUnit::release(){
	if(!m_locked)
		return;

	m_freeTextureUnits.push_front(m_index);
	m_index = -1;
	m_locked = false;
}
