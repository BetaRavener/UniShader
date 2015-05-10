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
#ifndef TEXTURE_H
#define TEXTURE_H

#include <UniShader/Config.h>
#include <UniShader/Utility.h>
#include <UniShader/ObjectBase.h>
#include <UniShader/Signal.h>
#include <UniShader/TextureUnit.h>
#include <UniShader/OpenGL.h>
#include <UniShader/GLSLType.h>

#include <memory>
#include <string>
#include <deque>

UNISHADER_BEGIN

class UniShader_API Texture : public SignalReceiver, public ObjectBase{
public:
    typedef std::shared_ptr<Texture> Ptr; //!< Shared pointer
    typedef std::shared_ptr<const Texture> PtrConst; //!< Shared pointer
    virtual const std::string& getClassName() const; //!< Get name of this class.
    ~Texture();

    //! Texture type.
    class TextureType{
    public:
        enum myEnum{ONE_DIM, //!< One dimensional texture.
                    TWO_DIM, //!< Two dimensional texture.
        };

        GLenum resolveGL()
        {
            switch(m_en)
            {
            case ONE_DIM: return GL_TEXTURE_1D;
            case TWO_DIM: return GL_TEXTURE_2D;
            }
            return 0;
        }
    private:
        myEnum m_en;
    public:
        TextureType(){}
        TextureType(const TextureType& ref):m_en(ref.m_en){}
        TextureType(myEnum en){ m_en = en; }
        TextureType& operator =(myEnum en){ m_en = en; return *this; }
        operator myEnum(){ return m_en; }
    };

    //! Create texture.
    /*!
        /return Texture.
    */
    static Ptr create(TextureType type);

    //! Get texture type.
    /*!
     * \return Returns texture type.
     */
    TextureType getType() const;

    //! Get if texture is mipmaped.
    /*!
     * \return True if texture is mipmaped.
     */
    bool isMipmaped() const;

    //! Get texture unit index.
    /*!
        Texture unit index is availible only if texture buffer is active.
        \return Texture unit index.
    */
    char getTextureUnitIndex() const;

    //! Set data.
    bool setData(const unsigned char* arr, unsigned int width, unsigned int height = 0);

    //! Set if texture will be mipmaped.
    /*!
     * \param mipmaped If true, the texture will be mipmaped.
     */
    void setMipmaping(bool mipmaped);

    //! Prepare.
    /*!
        Prepare texture for use.
        \return True if prepared successfully.
    */
    bool prepare();

    //! Activate.
    /*!
        Activate texture by locking a texture unit and modyfying its state.
        It is possible to activate texture multiple times.
    */
    void activate();

    //! Deactivate.
    /*!
        Deactivate texture by releasing texture unit.
        Texture unit is released only if all Activate() calls were matched by Deactivate().
    */
    void deactivate();

    //! Handle incoming signal.
    /*!
        \param signalID Signal identifier.
        \param callerPtr Pointer to object sending signal.
        \return True if handled.
    */
    virtual bool handleSignal(unsigned int signalID, const ObjectBase* callerPtr);
private:
    Texture(TextureType type);

    TextureUnit m_unit;
    TextureType m_type;
    unsigned int m_texture;
    unsigned int m_activeCount;
    bool m_mipmaped;
    bool m_prepared;
};

UNISHADER_END

#endif
