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
#include "Graphics3D.h"
#include "Font.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Material.h"

void TriangleScreen::Start(){
	CCScene::Start();
	
	triangle_shader = new Shader("gfx3D/shaders/simple.vert", "gfx3D/shaders/simple.frag");

	triangle_material = new Material(triangle_shader);

	VertexBuffer* vertexBuffer = new VertexBuffer();
	
	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 5.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = -5.f;  verticesData[1].y = -5.f;  verticesData[1].z = 0.0f;
	verticesData[2].x = 5.f;  verticesData[2].y = -5.f;  verticesData[2].z = 0.0f;

	vertexBuffer->PushData((CRByte*)&verticesData[0], 3 * sizeof(Vector3D));

	CRArray<unsigned int> indices = { 0, 1, 2 };

	triangle = new Mesh(vertexBuffer, indices, indices.size());
	triangle->SetMaterial(triangle_material);

	//buffer object
	/*
	SAFE(glGenBuffers(1, &vboId));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
	SAFE(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));*/
}

void TriangleScreen::Stop(){
	CCScene::Stop();
	delete triangle_shader;
	delete triangle_material;
	delete triangle;
	
	//SAFE(glDeleteBuffers(1, &vboId));
}

void TriangleScreen::Update(float sec){

	triangle->Draw();
	/*
	SAFE(glUseProgram(triangle_shader->program));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, vboId));
	if(shader->aPosition != -1)	{
		SAFE(glEnableVertexAttribArray(shader->aPosition));
		SAFE(glVertexAttribPointer(shader->aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0));
	}
	if(shader->uColor != -1){
		SAFE(glUniform4f(shader->uColor, 1.f, 0.f, 0.f, 0.f));
	}
	if(shader->uMVP != -1){
		static float angle = 0;
		angle += 90 * sec;
		Matrix translate = Matrix::CreateIdentity();
		Matrix rotate = Matrix::CreateIdentity();
		//rotate.SetRotationY(angle);
		Matrix mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * rotate;
		mvp = mvp.Transpose();
		SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
	}
	SAFE(glDrawArrays(GL_TRIANGLES, 0, 3));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	
	CCScene::Update(sec);
	
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}*/
}