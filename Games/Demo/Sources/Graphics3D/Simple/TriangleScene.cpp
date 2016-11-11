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
#include "TriangleScene.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "Mesh.h"

void TriangleScene::Start(){
	CameraControlsScreen::Start();

	shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	shader->Compile();
	material = new Material(shader);
	material->SetPropertyValue("Color", (void*)(&Color::Red));

	VertexBuffer* vertexBuffer = new VertexBuffer();
	
	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 1.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = -1.f;  verticesData[1].y = -1.f;  verticesData[1].z = 0.0f;
	verticesData[2].x = 1.f;  verticesData[2].y = -1.f;  verticesData[2].z = 0.0f;

	vertexBuffer->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	CRArray<U32> indices = { 0, 1, 2 };

	triangle = new Mesh(vertexBuffer, indices, indices.size());
	triangle->SetMaterial(material);
}

void TriangleScene::Stop(){
	delete triangle;
	delete material;
	delete shader;
	CameraControlsScreen::Stop();
}

void TriangleScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	triangle->Draw();
}