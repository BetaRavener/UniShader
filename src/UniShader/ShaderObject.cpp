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

#include <UniShader/ShaderObject.h>
#include <UniShader/OpenGL.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using UNISHADER_NAMESPACE;

ShaderObject::ShaderObject():
m_shaderObjectID(0),
m_type(Type::NONE),
m_compilationStatus(CompilationStatus::PENDING_COMPILATION){

}

const std::string& ShaderObject::getClassName() const{
	static const std::string name("us::ShaderObject");
	return name;
}

ShaderObject::~ShaderObject(){
}

ShaderObject::Ptr ShaderObject::create(){
	Ptr ptr(new ShaderObject);
	return ptr;
}

bool ShaderObject::loadFile(const std::string fileName, Type shaderType){
	clearGLErrors();
	m_compilationStatus = CompilationStatus::PENDING_COMPILATION;
	if(glIsShader(m_shaderObjectID))
		glDeleteShader(m_shaderObjectID);

        std::string::size_type dotPos;
	std::string code, extension;

	switch(shaderType){
	case Type::NONE:{
		//auto recognition
		dotPos = fileName.rfind(".");
		extension = fileName.substr(dotPos+1, fileName.size()-(dotPos+1));

		if(extension == "frag")
			m_type = Type::FRAGMENT;
		else if(extension == "vert")
			m_type = Type::VERTEX;
		else if(extension == "geom"){
			m_type = Type::GEOMETRY;
		}
		else{
			m_type = Type::UNRECOGNIZED;
			return FAILURE;
		}
		break;
	}
	case Type::FRAGMENT:
	case Type::VERTEX:
	case Type::GEOMETRY:
		m_type = shaderType;
		break;
	default:
		m_type = Type::UNRECOGNIZED;
		return FAILURE;
	}
	
	switch(m_type){
	case Type::FRAGMENT:
		m_shaderObjectID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case Type::VERTEX:
		m_shaderObjectID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case Type::GEOMETRY:
		//Find out if geometry shader is supported
        if(!glewIsSupported("GL_EXT_geometry_shader4")){
			std::cerr << "ERROR: Geometry shader is not supported by graphics card" << std::endl;
			m_type = Type::UNRECOGNIZED;
			return FAILURE;
		}
		else
			m_shaderObjectID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
    default:
        std::cerr << "ERROR: Invalid or unrecognized shader object type" << std::endl;
        break;
	}
	if(printGLError())
		return FAILURE;

    if(!readShaderSource(fileName,code))
        return FAILURE;
		
	const char *source = code.c_str();
	glShaderSource(m_shaderObjectID, 1, &source, NULL);
	if(printGLError())
		return FAILURE;

	m_compilationStatus = CompilationStatus::PENDING_COMPILATION;
	sendSignal(SignalID::CHANGED, this);

    return SUCCESS;
}

bool ShaderObject::loadCode(const std::string code, Type shaderType){
	clearGLErrors();
	m_compilationStatus = CompilationStatus::PENDING_COMPILATION;
	if(glIsShader(m_shaderObjectID))
		glDeleteShader(m_shaderObjectID);

	switch(shaderType){
	case Type::FRAGMENT:
	case Type::VERTEX:
	case Type::GEOMETRY:
		m_type = shaderType;
		break;
	default:
		m_type = Type::UNRECOGNIZED;
		return FAILURE;
	}
	
	switch(m_type){
	case Type::FRAGMENT:
		m_shaderObjectID = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case Type::VERTEX:
		m_shaderObjectID = glCreateShader(GL_VERTEX_SHADER);
		break;
	case Type::GEOMETRY:
		//Find out if geometry shader is supported
		if(getGLExtensions().find("GL_EXT_geometry_shader4") == std::string::npos){
			std::cerr << "ERROR: Geometry shader is not supported by graphics card" << std::endl;
			m_type = Type::UNRECOGNIZED;
			return FAILURE;
		}
		else
			m_shaderObjectID = glCreateShader(GL_GEOMETRY_SHADER);
		break;
     default:
        std::cerr << "ERROR: Invalid or unrecognized shader object type" << std::endl;
        break;
	}
	if(printGLError())
		return FAILURE;
		
	const char *source = code.c_str();
	glShaderSource(m_shaderObjectID, 1, &source, NULL);
	if(printGLError())
		return FAILURE;

	m_compilationStatus = CompilationStatus::PENDING_COMPILATION;
	sendSignal(SignalID::CHANGED, this);

    return SUCCESS;
}

bool ShaderObject::ensureCompilation(){
	if(m_compilationStatus == CompilationStatus::PENDING_COMPILATION)
		return compile();
	else{
		if(m_compilationStatus == CompilationStatus::SUCCESSFUL_COMPILATION)
			return SUCCESS;
		else
			return FAILURE;
	}
}

unsigned int ShaderObject::getGlID() const{
	return m_shaderObjectID;
}

ShaderObject::Type ShaderObject::getType() const{
	return m_type;
}

ShaderObject::CompilationStatus ShaderObject::getCompilationStatus() const{
	return m_compilationStatus;
}

bool ShaderObject::compile(){
	clearGLErrors();

	GLint compileStatus;

	if(m_type == Type::UNRECOGNIZED){
		std::cerr << "ERROR: Compiling unrecognized shader type" << std::endl;
		return FAILURE;
	}
	else if(m_type == Type::NONE){
		std::cerr << "ERROR: Shader was not loaded before compiling" << std::endl;
		return FAILURE;
	}

	glCompileShader(m_shaderObjectID);
    printGLError();
    glGetShaderiv(m_shaderObjectID, GL_COMPILE_STATUS, &compileStatus);
	printShaderInfoLog();

	if(compileStatus == GL_TRUE){
		m_compilationStatus = CompilationStatus::SUCCESSFUL_COMPILATION;
		sendSignal(SignalID::RECOMPILED, this);
		return SUCCESS;
	}
	else{
		m_compilationStatus = CompilationStatus::FAILED_COMPILATION;
		std::cerr << "ERROR: Shader object compilation failed" << std::endl;
		sendSignal(SignalID::RECOMPILED, this);
		return FAILURE;
	}
}

bool ShaderObject::printShaderInfoLog() const{
	clearGLErrors();

    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(m_shaderObjectID, GL_INFO_LOG_LENGTH, &infologLength);
	printGLError();  

    if(infologLength > 0){
        infoLog = (char *)malloc(infologLength);
        if(infoLog == NULL){
			std::cerr << "ERROR: Could not allocate InfoLog buffer" << std::endl;
            return 1;
        }

        glGetShaderInfoLog(m_shaderObjectID, infologLength, &charsWritten, infoLog);
		printGLError();

        std::cout << "Shader InfoLog:" << std::endl << infoLog <<std::endl<<std::endl;
        free(infoLog);
    }

	return 0;
}

int ShaderObject::getShaderSize(const std::string &shaderName) const{
	int count = -1;

	std::ifstream fin;
	fin.open(shaderName.data(),std::ios::binary);
    if(!fin)
        return -1;
	fin.seekg(0, std::ios::end);
	count = (int)fin.tellg();
	fin.close();

    return count;
}

bool ShaderObject::readShaderSource(const std::string& fileName, std::string& shaderText){
	std::ifstream fin;
	std::string shaderName;
    int count;

    // Open the file
	fin.open(fileName.data(),std::ios::binary);
    if(!fin){
		std::cerr << "ERROR: Failed to open " << fileName << std::endl;
        return FAILURE;
	}

    // Get the shader from a file.
	count = getShaderSize(fileName);
	shaderText.resize(count);

	if(fin.read(&shaderText[0],count)){
		if(count == 0){
			std::cerr << "ERROR: File " << fileName << " is empty" << std::endl;
			return FAILURE;
		}
		else
			translateLiterals(shaderText);
	}
	else{
		std::cerr << "ERROR: Failed to read " << fileName << std::endl;
		return FAILURE;
	}

	fin.close();
    return SUCCESS;
}

bool ShaderObject::translateLiterals(std::string &shaderText){
        std::string::size_type pos, end;

	//check for SwDouble switch
	if((pos = shaderText.find("//#SwDouble")) != std::string::npos){
		std::istringstream iss;
		std::ostringstream oss;
		std::string rep;
		double num;
		const unsigned int *p = (unsigned int*)&num;

		shaderText.erase(pos,11);
		pos = 0;

		while((pos = shaderText.find("SwDouble(\"", pos)) != std::string::npos){
			end = shaderText.find("\")", pos+10);

			if(end == std::string::npos){
				std::cerr << "ERROR: Translating literal failed - end of SwDouble not found" << std::endl;
				return FAILURE;
			}
			else{
				iss.clear();
				iss.str(shaderText.substr(pos+10,(end-(pos+10))));
				iss >> num;

				oss << *p;
				rep = "uvec2(" + oss.str() + ","; oss.str("");
				oss << *(p+1);
				rep += oss.str(); 
				rep+= ")"; 
				oss.str("");

				shaderText.replace(pos+9,(end+1-(pos+9)),rep);
			}

			pos = end;
		}
		p = 0;
	}

	return SUCCESS;
}
