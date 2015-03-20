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
	
#include "Graphics.h"

Graphics::Graphics(Game* game){
	this->game = game;
	Launcher* launcher = game->launcher;
	glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, launcher->GetTargetWidth(), launcher->GetTargetHeight(), 0, 1, -1);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Graphics::Clear(float r, float g, float b){
	glClearColor(r, g, b, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Graphics::Test(){
	GLfloat width = game->launcher->GetTargetWidth();
	GLfloat height = game->launcher->GetTargetHeight();

	//	GLfloat verts[] = {	0.0f, 0.0f,
    //                        width - 1, 0.0f,
    //                        width/2, height - 1 };
    GLfloat verts[] = { width - 1, height - 1,
        0, height - 1,
        width / 2, 0 };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

/*
void Graphics::Init(Launcher* launcher){
	glViewport(0, 0, 550, 900);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, 550, 900, 0, 1, -1);
	char buf[128];
	launcher->LogIt("You have successfully initialized Graphics Class");
	sprintf(buf, "Target width: %d", launcher->GetTargetWidth());
	launcher->LogIt(buf);
	sprintf(buf, "Target height: %d", launcher->GetTargetHeight());
	launcher->LogIt(buf);
}

void Graphics::Test(){
    glClearColor(0, 0.25, 0.25, 0.5);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	GLfloat verts[] = {	0.0f, 0.0f,
						319.0f, 0.0f,
						160.0f, 479.0f };
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
*/