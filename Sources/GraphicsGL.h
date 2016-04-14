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
#include "Launcher.h"

#ifdef WIN
#ifdef OPENGL
#include "Platform\Windows\glew\glew.h"
#include "Platform\Windows\glew\wglew.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#elif GLES
#include "Platform\Windows\GLES2\gl2.h"
#include "Platform\Windows\EGL\egl.h"
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libGLESv2.lib")
#endif
#undef LoadImage
#undef DrawText
#elif ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

#ifdef CROSS_DEBUG
#define SAFE(X) \
{\
	(X);      \
	GraphicsGL::CheckGLError(); \
}
#else
#define SAFE(X) X;
#endif

namespace cross{

class Shader;

class GraphicsGL{
public:
	static void CheckGLError();

	GraphicsGL();
	~GraphicsGL();


	Shader* GetShader(unsigned int type);
	GLuint ComplileShader(string filename);
	void DeleteShader(GLuint shader);
	GLuint CreateProgram();
	void DeleteProgram(GLuint program);
	void AttachShader(GLuint program, GLuint shader);
	void CompileProgram(GLuint program);
	void UseShader(Shader* shaders);
private:
	CRArray<Shader*> shaders;

	void WindowResizeHandle(int width, int height);
};

}