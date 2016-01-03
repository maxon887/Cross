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
#include "Screen2D.h"
#include "Cross.h"
#include "Graphics2D.h"
#include "Matrix.h"

const GLuint NumVertices = 3;

static GLfloat vertices[NumVertices * 3] = { 250.0f, 500.0f, 0.0f,
200.0f, 400.0f, 0.0f,
300.0f, 400.0f, 0.0f };
	
void Screen2D::Start(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	logo = gfx2D->LoadImage("Logo.png");
	logo->SetPosition(Vector2D(256, 256));

}

void Screen2D::Update(float sec){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gfx2D->DrawTargetImage(256, 256, logo);
}