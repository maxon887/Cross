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
#include "PrimitiveShaders.h"

using namespace cross;

PrimitiveShaders::PrimitiveShaders(){
	vertex_shader = gfxGL->ComplileShader("Engine/primitive.vert");
	fragment_shader = gfxGL->ComplileShader("Engine/primitive.frag");
	program = gfxGL->CreateProgram();
	gfxGL->AttachShader(program, vertex_shader);
	gfxGL->AttachShader(program, fragment_shader);
	gfxGL->CompileProgram(program);

	aPositionLoc = glGetAttribLocation(program, "aPosition");
	uProjectionLoc = glGetUniformLocation(program, "uProjection");
	uColor = glGetUniformLocation(program, "uColor");
}

PrimitiveShaders::~PrimitiveShaders(){
	gfxGL->DeleteShader(vertex_shader);
	gfxGL->DeleteShader(fragment_shader);
	gfxGL->DeleteProgram(program);
}