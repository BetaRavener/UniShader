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

#include <UniShader/Texture.h>
#include <UniShader/OpenGL.h>

#include <iostream>

using UNISHADER_NAMESPACE;

Texture::Texture(TextureType type):
m_type(type),
m_texture(0),
m_activeCount(0),
m_prepared(false){
    clearGLErrors();

    glGenTextures(1, &m_texture);
    printGLError();
}

const std::string& Texture::getClassName() const{
    static const std::string name("us::Texture");
    return name;
}

Texture::~Texture(){
    glDeleteTextures(1, &m_texture);
    printGLError();
    m_unit.release();
}

Texture::Ptr Texture::create(TextureType type){
    clearGLErrors();

    Ptr ptr(new Texture(type));
    if((ptr->m_texture == 0))
        return 0;

    ptr->m_unit.lock();
    if(!ptr->m_unit.makeActive()){
        ptr->m_unit.release();
        return 0;
    }
    //first bind is used to specialize texture
    glBindTexture(type.resolveGL(), ptr->m_texture);
    glBindTexture(type.resolveGL(), 0);

    //TODO: Unexpected texture problems may be caused by this
    ptr->m_unit.release();

    if(printGLError())
        return 0;

    return ptr;
}

Texture::TextureType Texture::getType() const{
    return m_type;
}

bool Texture::isMipmaped() const
{
    return m_mipmaped;
}

char Texture::getTextureUnitIndex() const{
    if(m_activeCount == 0){
        std::cerr << "ERROR: Texture buffer must be activated before querying texture unit index" << std::endl;
        return -1;
    }
    else
        return m_unit.getIndex();
}

bool Texture::setData(const unsigned char *arr, unsigned int width, unsigned int height)
{
    m_unit.lock();
    if(!m_unit.makeActive())
        return FAILURE;

    glBindTexture(m_type.resolveGL(), m_texture);

    switch(m_type)
    {
    case TextureType::ONE_DIM:
        glTexImage1D(m_type.resolveGL(), 0, GL_RGBA8, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, arr);
        break;
    case TextureType::TWO_DIM:
        glTexImage2D(m_type.resolveGL(), 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, arr);
        break;
    }

    m_prepared = false;
    return SUCCESS;
}

void Texture::setMipmaping(bool mipmaped)
{
    m_mipmaped = mipmaped;
    m_prepared = false;
}

bool Texture::prepare(){
    clearGLErrors();

    if(!m_prepared){
        m_unit.lock();
        if(!m_unit.makeActive())
            return FAILURE;

        glBindTexture(m_type.resolveGL(), m_texture);

        if (m_mipmaped)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexParameteri(m_type.resolveGL(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(m_type.resolveGL(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }

    return SUCCESS;
}

void Texture::activate(){
    clearGLErrors();

    if(!prepare())
        return;

    m_unit.lock();
    if(!m_unit.makeActive())
        return;
    glBindTexture(m_type.resolveGL(), m_texture);

    m_activeCount++;
}

void Texture::deactivate(){
    clearGLErrors();

    if(m_activeCount != 0){
        m_activeCount--;
        if(m_activeCount == 0){
            m_unit.makeActive();
            glBindTexture(m_type.resolveGL(), 0);
            m_unit.release();
        }
    }
}

bool Texture::handleSignal(unsigned int signalID, const ObjectBase* callerPtr)
{
    return FAILURE;
}
