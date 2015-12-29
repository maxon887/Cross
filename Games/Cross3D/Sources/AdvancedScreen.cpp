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
#include "AdvancedScreen.h"
#include "Launcher.h"
#include "Matrix.h"

static GLfloat triangle_vers[] = {
	 0.0f,  0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f
};

GLuint VBO;

AdvancedScreen::~AdvancedScreen(){
	delete vertex_shader;
	delete fragment_shader;
}

void AdvancedScreen::Start(){
	vertex_shader = new VertexShader("advanced.vert");
	fragment_shader = new Shader("advanced.frag");
	gfx3D->AttachShader(vertex_shader);
	gfx3D->AttachShader(fragment_shader);
	program = gfx3D->CompileProgram();
	vertex_shader->Initialize(program);
	

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vers), triangle_vers, GL_STATIC_DRAW);
	glVertexAttribPointer(vertex_shader->aPositionLoc, 4, GL_FLOAT, GL_FLOAT, 0, (void*)0);
	glEnableVertexAttribArray(vertex_shader->aPositionLoc);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triangle_mode = Matrix::CreateTranslation(Vector3D(0.5f, 0.0f, 0.0f));
}

void AdvancedScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(vertex_shader->uModelLoc, 1, GL_TRUE, triangle_mode.GetData());
	glDrawArrays(GL_TRIANGLES, 0, 3);
}