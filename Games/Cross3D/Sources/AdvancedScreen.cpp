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

static GLfloat triangle_vers[] = {
	 0.0f,  0.5f,
	 0.5f, -0.5f,
	-0.5f, -0.5f
};

void AdvancedScreen::Start(){
	Shader* vertexShader = new Shader("advanced.vert");
	Shader* fragmentShader = new Shader("advanced.frag");
	gfx3D->AttachShader(vertexShader);
	gfx3D->AttachShader(fragmentShader);
	program = gfx3D->CompileProgram();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void AdvancedScreen::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}