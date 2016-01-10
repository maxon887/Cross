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

class Shader;

class Graphics{
//User module
public:
	Graphics();
	void AttachShader(Shader* shader);
	GLuint CompileProgram();

	//void DrawPoint(Vector2D p, Color c);
protected:
	GLuint program;
//Framework module. You don't need call any of this methods or modify variable
};

}