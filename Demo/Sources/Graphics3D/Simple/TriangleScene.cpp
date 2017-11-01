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
	CameraControlsScene::Start();
	MenuBar::Init();

	shader = GetShader("Engine/Shaders/Simple.sha");
	material = new Material(shader);
	material->SetPropertyValue("Color", Color::Red);

	VertexBuffer* vertexBuffer = new VertexBuffer();
	
	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 1.f;  verticesData[0].z = 0.0f;
    verticesData[1].x = 1.f;  verticesData[2].y = -1.f;  verticesData[2].z = 0.0f;
	verticesData[2].x = -1.f;  verticesData[1].y = -1.f;  verticesData[1].z = 0.0f;

	vertexBuffer->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	const Array<GLushort> indices = { 0, 1, 2 };

	triangle = new Mesh();
	triangle->PushData(vertexBuffer, indices);
	triangle->TransferVideoData();
	triangle->SetMaterial(material);
	triangle->SetFaceCullingEnabled(false);
    delete vertexBuffer;
}

void TriangleScene::Stop(){
	delete triangle;
	delete material;
	MenuBar::Release();
	CameraControlsScene::Stop();
}

void TriangleScene::Update(float sec){
	CameraControlsScene::Update(sec);
	MenuBar::Update(sec);
	MenuBar::ShowMenu();
	triangle->Draw();
}