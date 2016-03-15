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
#include "TriangleScreen.h"
#include "GraphicsGL.h"
#include "Input.h"
#include "Game.h"
#include "Graphics2D.h"
#include "Font.h"

struct Vertex
{
	Vector3D pos;
};

void TriangleScreen::Start(){
	shader = new TriangleShaders();
	camera = new Camera3D();
	camera->SetPosition(Vector3D(0.f, 0.f, -1.f));
	debug_font = gfx2D->GetDefaultFont()->Clone();
	debug_font->SetSize(25.f);

	Vertex verticesData[3];

	verticesData[0].pos.x = 0.0f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z = 0.0f;
	verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;

	//buffer object
	SAFE(glGenBuffers(1, &vboId));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
	SAFE(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void TriangleScreen::Stop(){
	delete shader;
	delete camera;
	delete debug_font;
	SAFE(glDeleteBuffers(1, &vboId));
}

void TriangleScreen::Update(float sec){
	camera->Update(sec);


	SAFE(glUseProgram(shader->program));
	if(shader->aPosition != -1)
	{
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if(shader->uMVP != -1)
	{
		static float angle = 0;
		angle += 90 * sec;
		Matrix translate = Matrix::CreateIdentity();
		Matrix rotate = Matrix::CreateIdentity();
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}
	SAFE(glDrawArrays(GL_TRIANGLES, 0, 3));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//debug camera
	char buffer[256];
	Vector3D camPos = camera->GetPosition();
	sprintf(buffer, "Camera Position: %f, %f, %f", camPos.x, camPos.y, camPos.z);
	gfx2D->DrawText(Vector2D(5.f, 3.f), buffer, debug_font);
	Vector3D camDir = camera->GetDirection();
	sprintf(buffer, "Camera Direction: %f, %f, %f", camDir.x, camDir.y, camDir.z);
	gfx2D->DrawText(Vector2D(5.f, 3.f + debug_font->GetSize()), buffer, debug_font);
	//exit screen
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}