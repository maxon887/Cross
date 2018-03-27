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
	CROSS_ASSERT(!glewInit(), "Unable to initialize GLEW");
#endif

	system->LogIt("\tRenderer - %s", glGetString(GL_RENDERER));
	system->LogIt("\tOpenGL version - %s", glGetString(GL_VERSION));
	const char* shaderVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	system->LogIt("\tGLSL version - %s", shaderVersion);

    shader_version = shaderVersion;
	std::size_t const n = shader_version.find_first_of("0123456789.");
	if (n != std::string::npos) {
		std::size_t const m = shader_version.find_first_not_of("0123456789.", n);
		shader_version = shader_version.substr(n, m != std::string::npos ? m-n : m);
	} else {
		CROSS_ASSERT(false, "Can not obtain shader version");
	}
	shader_version.erase(remove(shader_version.begin(), shader_version.end(), '.'));
	
	GLint value;
	SAFE(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value));
	system->LogIt("\tMax Vertex Uniforms: %d", value);

	SAFE(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value));
	system->LogIt("\tMax Fragment Uniforms: %d", value);

	SAFE(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
	system->LogIt("\tMax Vertex Attributes: %d", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value));
	system->LogIt("\tMax Texture Size: %d", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value));
	system->LogIt("\tMax Texture Units: %d", value);

	system->LogIt("\tDevice DPI - %f", system->GetScreenDPI());

	system->WindowResized.Connect(this, &GraphicsGL::WindowResizeHandle);

	SAFE(glCullFace(GL_FRONT));
}

const String& GraphicsGL::GetShaderVersion() const {
	return shader_version;
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height) {
	SAFE(glViewport(0, 0, width, height));
}
