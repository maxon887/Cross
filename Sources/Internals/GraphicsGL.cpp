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
		CROSS_ASSERT(false, "Rendering error number: # in # : #", error, file, line);
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

#ifdef OPENGL
	CROSS_ASSERT(!glewInit(), "Unable to initialize GLEW");
#endif

	system->LogIt("\tRenderer - #", (const char*)glGetString(GL_RENDERER));
	system->LogIt("\tOpenGL version - #", (const char*)glGetString(GL_VERSION));
	shader_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	system->LogIt("\tGLSL version - " + shader_version);

	S32 first = shader_version.FindFirstOf("0123456789.");
	if(first != -1) {
		S32 last = shader_version.FindNonFirstOf("0123456789.", first);
		if(last != -1) {
			shader_version.Cut(first, last);
		} else {
			shader_version.Cut(first, shader_version.Length());
		}
	} else {
		CROSS_ASSERT(false, "Can not obtain shader version");
	}
	shader_version.Remove(".");
	
	GLint value;
	SAFE(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value));
	system->LogIt("\tMax Vertex Uniforms: #", value);

	SAFE(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value));
	system->LogIt("\tMax Fragment Uniforms: #", value);

	SAFE(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
	system->LogIt("\tMax Vertex Attributes: #", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value));
	system->LogIt("\tMax Texture Size: #", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value));
	system->LogIt("\tMax Texture Units: #", value);

	system->LogIt("\tDevice DPI - #", system->GetScreenDPI());

	system->WindowResized.Connect(this, &GraphicsGL::WindowResizeHandle);

	SAFE(glCullFace(GL_FRONT));
}

const String& GraphicsGL::GetShaderVersion() const {
	return shader_version;
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height) {
	SAFE(glViewport(0, 0, width, height));
}
