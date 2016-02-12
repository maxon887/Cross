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
#include "SpriteShaders.h"

using namespace cross;

SpriteShaders::SpriteShaders(){
	vertex_shader = gfxGL->ComplileShader("Engine/Shaders/sprite.vert");
	fragment_shader = gfxGL->ComplileShader("Engine/Shaders/sprite.frag");
	program = gfxGL->CreateProgram();
	gfxGL->AttachShader(program, vertex_shader);
	gfxGL->AttachShader(program, fragment_shader);
	gfxGL->CompileProgram(program);

	aPositionLoc = glGetAttribLocation(program, "aPosition");
	aTexCoordLoc = glGetAttribLocation(program, "aTexCoord");
	uModelLoc = glGetUniformLocation(program, "uModel");
	uProjectionLoc = glGetUniformLocation(program, "uProjection");
	uMonochrome = glGetUniformLocation(program, "uMonochrome");
	uColor = glGetUniformLocation(program, "uColor");
}

SpriteShaders::~SpriteShaders(){
	gfxGL->DeleteShader(vertex_shader);
	gfxGL->DeleteShader(fragment_shader);
	gfxGL->DeleteProgram(program);
}