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
#include "HelloTriangleScreen.h"
#include "Graphics3D.h"
#include "Launcher.h"
#include <vector>

const GLuint NumVertices = 3;

GLfloat vertices[NumVertices][3] = {	 0.0f,	0.5f, 0.0f,
										-0.5f, -0.5f, 0.0f,
										 0.5f, -0.5f, 0.0f	};

void HelloTriangleScreen::Start(){
	Shader* vertShader = new Shader("triangles.vert");
	Shader* fragShader = new Shader("triangles.frag");
	gfx3D->AttachShader(vertShader);
	gfx3D->AttachShader(fragShader);
	gfx3D->CompileProgram();

	//GLuint program = gfx3D->LoadShaders(shaders);
	//glUseProgram(program);

	//glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
}

void HelloTriangleScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	//glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
}
