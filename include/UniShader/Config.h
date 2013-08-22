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

//Identify the operating system
#if defined(_WIN32) || defined(__WIN32__)

    // Windows
    #define UNISHADER_SYSTEM_WINDOWS

#elif defined(linux) || defined(__linux)

    // Linux
    #define UNISHADER_SYSTEM_LINUX

#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)

    // MacOS
    #define UNISHADER_SYSTEM_MACOS

#endif

// Identify the compiler and create import/export macro
#ifndef UNISHADER_STATIC

    #ifdef UNISHADER_SYSTEM_WINDOWS

        #ifdef UNISHADER_EXPORTS

            // Export macro for library side
            #define UniShader_API __declspec(dllexport)

        #else

            // Import macro for client side
            #define UniShader_API __declspec(dllimport)

        #endif

        // Disable warning about "implicit exporting" of instantiated templates
        #ifdef _MSC_VER

            #pragma warning(disable : 4251)

        #endif

    #else // Linux, Mac OS X

        #if __GNUC__ >= 4

            // gcc 4 has special keywords for showing/hidding symbols
            #define UniShader_API __attribute__ ((__visibility__ ("default")))

        #else

            // gcc < 4 has no mechanism to explicitely hide symbols, everything's exported
            #define UniShader_API

        #endif

    #endif

#else

    // Static build doesn't need export macros
    #define UniShader_API

#endif
