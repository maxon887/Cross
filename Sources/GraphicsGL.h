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
#pragma once
#include "Cross.h"

#ifdef WIN
#ifdef GLES
#include "Platform\Windows\GLES2\gl2.h"
#include "Platform\Windows\EGL\egl.h"
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#else
#include "Platform\Windows\glew\glew.h"
#include "Platform\Windows\glew\wglew.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#endif
#undef LoadImage
#undef DrawText
#elif ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif EDITOR
#include "Platform\Windows\glew\glew.h"
#include "Platform\Windows\glew\wglew.h"
#endif

//doesn't properly work for windows emulator
#if defined(CROSS_DEBUG) && !defined(GLES)
#define SAFE(X) \
{\
	(X);      \
	GraphicsGL::CheckGLError(__FILE__, __LINE__); \
}
#else
#define SAFE(X) X;
#endif

namespace cross{

enum DefaultShader{
	TEXTURE,
	MONOCHROME,		//same as TEXTURE but only R channel used
	SIMPLE,
	MULTI_LIGHT,
	NONE
};

/* Class containing specific OpengGL features */
class GraphicsGL{
public:
	static void CheckGLError(const char* file, U32 line);

	GraphicsGL();
	~GraphicsGL();

	void Start();

	void PreProcessFrame();
	void PostProcessFrame();

	void UseShader(Shader* shaders);
	Shader* GetShader(DefaultShader type);

private:
	//off screen rendering components
	GLuint framebuffer;
	GLuint colorbuffer;
	GLuint depthbuffer;
	GLuint quadVBO;
	GLuint quadEBO;

	void WindowResizeHandle(S32 width, S32 height);
};

}