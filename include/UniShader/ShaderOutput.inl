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

#include <UniShader/Utility.h>
#include <iostream>

UNISHADER_BEGIN

template <typename T>
typename std::shared_ptr<const Buffer<T>> ShaderOutput::getInterleavedBuffer(){
	if(m_interleaved)
		return std::static_pointer_cast<const Buffer<T>>(m_interleavedBuffer);
	else{
		std::cerr << "ERROR: Output isn't set to interleaved mode" << std::endl;
		return 0;
	}
}

UNISHADER_END
