#include "TriangleScene.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"

void TriangleScene::Start(){
	DemoScene::Start();

	Material* material = GetMaterial("Materials/SimpleRed.mat");

	VertexBuffer* vertexBuffer = CREATE VertexBuffer();

	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 1.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = 1.f;  verticesData[2].y = -1.f;  verticesData[2].z = 0.0f;
	verticesData[2].x = -1.f;  verticesData[1].y = -1.f;  verticesData[1].z = 0.0f;

	vertexBuffer->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	Array<GLushort> indices(3, 0);
	indices.Add(0);
	indices.Add(1);
	indices.Add(2);

	Mesh* triangle = CREATE Mesh();
	triangle->PushData(vertexBuffer, indices);
	triangle->InitializeVideoData();
	triangle->SetMaterial(material);
	triangle->EnableFaceCulling(false);
	
	delete vertexBuffer;

	Entity* entity = CREATE Entity("Triangle");
	entity->AddComponent(CREATE Transform());
	entity->AddComponent(triangle);
	AddEntity(entity);
}