#include "AxisScene.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"

void AxisScene::Start(){
	DemoScene::Start();

	Material* material = GetMaterial("Materials/SimpleGreen.mat");

	VertexBuffer* vertexBuffer = CREATE VertexBuffer();

	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 1.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = 0.3f;  verticesData[2].y = 0.f;  verticesData[2].z = 0.0f;
	verticesData[2].x = -0.3f;  verticesData[1].y = 0.f;  verticesData[1].z = 0.0f;

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

	Entity* entity = CREATE Entity("TriangleUP");
	entity->AddComponent(CREATE Transform());
	entity->AddComponent(triangle);
	AddEntity(entity);




	Material* simpleRed = GetMaterial("Materials/SimpleRed.mat");

	VertexBuffer* vertexBuffer2 = CREATE VertexBuffer();

	//Vector3D verticesData[3];
	verticesData[0].x = 1.0f;  verticesData[0].y = 0.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = 0.0f;  verticesData[2].y = 0.3f;  verticesData[2].z = 0.0f;
	verticesData[2].x = 0.0f;  verticesData[1].y = -0.3f;  verticesData[1].z = 0.0f;

	vertexBuffer2->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	Array<GLushort> indices2(3, 0);
	indices2.Add(0);
	indices2.Add(1);
	indices2.Add(2);

	Mesh* triangle2 = CREATE Mesh();
	triangle2->PushData(vertexBuffer2, indices2);
	triangle2->InitializeVideoData();
	triangle2->SetMaterial(simpleRed);
	triangle2->EnableFaceCulling(false);

	delete vertexBuffer2;

	Entity* entity2 = CREATE Entity("TriangleRight");
	entity2->AddComponent(CREATE Transform());
	entity2->AddComponent(triangle2);
	AddEntity(entity2);




	Material* simpleBlue = GetMaterial("Materials/SimpleBlue.mat");

	VertexBuffer* vertexBuffer3 = CREATE VertexBuffer();

	//Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 0.f;  verticesData[0].z = 1.0f;
	verticesData[1].x = 0.3f;  verticesData[2].y = 0.0f;  verticesData[2].z = 0.0f;
	verticesData[2].x = -0.3f;  verticesData[1].y = 0.0f;  verticesData[1].z = 0.0f;

	vertexBuffer3->PushData((Byte*)&verticesData[0], 3 * sizeof(Vector3D));

	Array<GLushort> indices3(3, 0);
	indices3.Add(0);
	indices3.Add(1);
	indices3.Add(2);

	Mesh* triangle3 = CREATE Mesh();
	triangle3->PushData(vertexBuffer3, indices3);
	triangle3->InitializeVideoData();
	triangle3->SetMaterial(simpleBlue);
	triangle3->EnableFaceCulling(false);

	delete vertexBuffer3;

	Entity* entity3 = CREATE Entity("TriangleForward");
	entity3->AddComponent(CREATE Transform());
	entity3->AddComponent(triangle3);
	AddEntity(entity3);

	Entity* gizmo = ImportModel("Models/Gizmo.fbx");
	Entity* xEntity = gizmo->FindChild("X");
	ApplyMaterial(xEntity, simpleRed);
	Entity* yEntity = gizmo->FindChild("Y");
	ApplyMaterial(yEntity, material);
	Entity* zEntity = gizmo->FindChild("Z");
	ApplyMaterial(zEntity, simpleBlue);
}