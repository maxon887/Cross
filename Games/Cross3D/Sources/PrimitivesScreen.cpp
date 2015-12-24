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
#include "PrimitivesScreen.h"
#include "Launcher.h"

static const GLuint NumVertices = 1;

static GLfloat vertices[2] = { 0.0f, 0.0f };

void PrimitivesScreen::Start(){
	Shader* vertShader = new Shader("primitives.vert");
	Shader* fragShader = new Shader("primitives.frag");
	gfx3D->AttachShader(vertShader);
	gfx3D->AttachShader(fragShader);
	gfx3D->CompileProgram();

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void PrimitivesScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	gfx3D->DrawPoint(Point(-0.5f, -0.5f), Color::Red);
	gfx3D->DrawPoint(Point(.5f, .5f), Color::Blue);
	gfx3D->DrawPoint(Point(0.5f, -0.5f), Color::Green);
	gfx3D->DrawPoint(Point(-0.5f, 0.5f), Color::Yellow);
}