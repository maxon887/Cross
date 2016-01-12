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
#include "Platform\Windows\glew\glew.h"
#include "Platform\Windows\glew\wglew.h"
#elif ANDROID
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

namespace cross{

class GraphicsGL{
public:
	GraphicsGL();
	GLuint ComplileShader(string filename);
	void DeleteShader(GLuint shader);
	GLuint CreateProgram();
	void DeleteProgram(GLuint program);
	void AttachShader(GLuint program, GLuint shader);
	void CompileProgram(GLuint program);
	void UseProgram(GLuint program);
};

}