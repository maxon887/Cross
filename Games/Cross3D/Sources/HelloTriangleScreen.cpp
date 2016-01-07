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
#include "GraphicsGL.h"
#include "Shader.h"
#include "Launcher.h"
#include <vector>
#include "Graphics2D.h"

const GLuint NumVertices = 3;

static GLfloat vertices[NumVertices * 3] = { 250.0f, 250, 0.0f,
											200.5f, 200.f, 0.0f,
											 350.5f, 200.5f, 0.0f		};

static GLshort indices[3] = { 0, 1, 2 };

void HelloTriangleScreen::Start(){
	Shader* vertShader = new Shader("triangles.vert");
	Shader* fragShader = new Shader("triangles.frag");
	
	//gfx2D->AttachShader(vertShader);
	//gfx2D->AttachShader(fragShader);
	//gfx2D->CompileProgram();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void HelloTriangleScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glDrawElements(GL_TRIANGLES, 3, GL_SHORT, indices);
	//glFlush();
}
