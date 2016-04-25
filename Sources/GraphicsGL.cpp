/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
#include "GraphicsGL.h"
#include "Launcher.h"
#include "File.h"
#include "Game.h"
#include "Shaders/SimpleShader.h"
#include "Shaders/MonochromeShader.h"
#include "Shaders/TextureShader.h"
#include "Shaders/LightCasterMaterialShader.h"
#include "Shaders/LightCasterDiffuseShader.h"
#include "Shaders/LightCasterDiffuseSpecularShader.h"
#include "Shaders/DirectionalLightShader.h"
#include "Shaders/PointLightShader.h"

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, unsigned int line) {
	GLenum err;
	err = glGetError();
	while(err != GL_NO_ERROR) {
		char* error = new char[255];
		switch(err) {
		case GL_INVALID_OPERATION:    
			strcpy(error, "INVALID_OPERATION");      
			break;
		case GL_INVALID_ENUM:     
			strcpy(error, "INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:     
			strcpy(error, "INVALID_VALUE");         
			break;
		case GL_OUT_OF_MEMORY:     
			strcpy(error, "OUT_OF_MEMORY");          
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  
			strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");  
			break;
		default: strcpy(error, "Unknown error");  
			break;
		}
		//launcher->LogIt("[ERROR] Rendering error number: %s in %s : %d", error, file, line);
		throw CrossException("OpenGL error: %s in %s : %d", error, file, line);
		//delete[] error;
		//err = glGetError();
}
}

GraphicsGL::GraphicsGL() : 
	shaders(Shader::Type::COUNT)	
{
		launcher->LogIt("GraphicsGL::GraphicsGL()");
#if defined (WIN) && defined(OPENGL)
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		launcher->LogIt("OpenGL " + to_string(magorV) + "." + to_string(minorV));
		if(glewInit()) {
			throw CrossException("Unable to initialize GLEW");
		}
#endif
		GLint maxVertexAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
		launcher->LogIt("Max Vertex Attributes: %d", maxVertexAttribs);

		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		launcher->LogIt("Max Texture Size: %d", maxTextureSize);

		GLint maxTextureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		launcher->LogIt("Max Texture Units: %d", maxTextureUnits);

		game->WindowResized += MakeDelegate(this, &GraphicsGL::WindowResizeHandle);
}

GraphicsGL::~GraphicsGL(){
	for(Shader* shader : shaders){
		delete shader;
	}
}

Shader* GraphicsGL::GetShader(unsigned int type){
	if(shaders[type] == nullptr){
		switch(type) {
		case Shader::Type::SIMPLE:
			shaders[type] = new SimpleShader();
			break;
		case Shader::Type::MONOCHROME:
			shaders[type] = new MonochromeShader();
			break;
		case Shader::Type::TEXTURE:
			shaders[type] = new TextureShader();
			break;
		case Shader::Type::LIGHT_CASTER_MATERIAL:
			shaders[type] = new LightCasterMaterialShader();
			break;
		case Shader::Type::LIGHT_CASTER_DIFFUSE:
			shaders[type] = new LightCasterDiffuseShader();
			break;
		case Shader::Type::LIGHT_CASTER_DIFFUSE_SPECULAR:
			shaders[type] = new LightCasterDiffuseSpecularShader();
			break;
		case Shader::Type::DIRECTIONAL_LIGHT:
			shaders[type] = new DirectionalLightShader();
			break;
		case Shader::Type::POINT_LIGHT:
			shaders[type] = new PointLightShader();
			break;
		default:
			throw CrossException("Unknown shader type");
		}
	}
	return shaders[type];
}

GLuint GraphicsGL::ComplileShader(string filename){
		//file loading part
		string extension = filename.substr(filename.find_last_of(".") + 1);
		GLuint type;
		if(extension == "vert") {
			type = GL_VERTEX_SHADER;
		} else if(extension == "frag") {
			type = GL_FRAGMENT_SHADER;
		} else {
			throw CrossException("Can't compile shader.\nUnknown file extension.");
		}
		File* file = launcher->LoadFile(filename);
		CRByte* source = new CRByte[file->size + 1]; // +1 for null terminated string
		memcpy(source, file->data, file->size);
		source[file->size] = 0;
		delete file;
		//shader compilling part
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const char**)&source, NULL);
		delete[] source;
		source = NULL;

		glCompileShader(handle);
		GLint compiled;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
		if(!compiled) {
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

			char* log = new char[len + 1];
			glGetShaderInfoLog(handle, len, &len, log);
			throw CrossException("Shader: %s\n%sShader", filename.c_str(), log);
		} else {
#ifdef CROSS_DEBUG
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
			if(len > 1){
				char* log = new char[len + 1];
				glGetShaderInfoLog(handle, len, &len, log);
				string msg(log);
				delete[] log;
				launcher->LogIt("Shader compilation:\n" + msg);
			}
#endif
		}
		return handle;
}

void GraphicsGL::DeleteShader(GLuint handle){
	glDeleteShader(handle);
}

GLuint GraphicsGL::CreateProgram(){
	return glCreateProgram();
}

void GraphicsGL::DeleteProgram(GLuint program){
	glDeleteProgram(program);
}

void GraphicsGL::AttachShader(GLuint program, GLuint shader){
	glAttachShader(program, shader);
}

void GraphicsGL::CompileProgram(GLuint program){
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		char* log = new char[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		throw CrossException("Shader compilation failed:\n %s", log);
	}
}

void GraphicsGL::UseShader(Shader* shader){
	if(shader){
		SAFE(glUseProgram(shader->program));
	}else{
		throw CrossException("Attempt to draw with NULL shader");
	}
}

void GraphicsGL::WindowResizeHandle(int width, int height){
	SAFE(glViewport(0, 0, width, height));
}