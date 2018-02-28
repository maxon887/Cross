/*	Copyright © 2018 Maksim Lukyanov

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
#include "System.h"

#include <algorithm>
#include <cstring>

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, U32 line) {
	GLenum err = glGetError();
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
		default: 
			strcpy(error, "Unknown error");  
			break;
		}
		CROSS_ASSERT(false, "Rendering error number: %s in %s : %d", error, file, line);
		delete[] error;
		err = glGetError();
	}
}

void GraphicsGL::ClearGLErrorBuffer() {
	GLenum err = glGetError();
	while(err != GL_NO_ERROR) {
		err = glGetError();
	}
}

GraphicsGL::GraphicsGL() {
		system->LogIt("GraphicsGL::GraphicsGL()");

#if defined(OPENGL) || defined(EDITOR)
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		system->LogIt("\tUsed OpenGL %d.%d", magorV, minorV);
		CROSS_ASSERT(!glewInit(), "Unable to initialize GLEW");
#else
		system->LogIt("\tUsed OpenGL ES 2.0");
#endif
		const Byte* shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		system->LogIt("\tSupported shader version %s", shaderVersion);
		String strV((const char*)shaderVersion);
		strV.erase(remove(strV.begin(), strV.end(), '.'));
		shaders_version = atoi(strV.c_str());
		
		GLint value;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value);
		system->LogIt("\tMax Vertex Uniforms: %d", value);

		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
		system->LogIt("\tMax Fragment Uniforms: %d", value);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
		system->LogIt("\tMax Vertex Attributes: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		system->LogIt("\tMax Texture Size: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		system->LogIt("\tMax Texture Units: %d", value);

		system->LogIt("\tDevice DPI - %f", system->GetScreenDPI());

		system->WindowResized.Connect(this, &GraphicsGL::WindowResizeHandle);

		SAFE(glCullFace(GL_FRONT));
}

U32 GraphicsGL::GetShaderVersion() const {
	return shaders_version;
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height) {
	SAFE(glViewport(0, 0, width, height));
}
