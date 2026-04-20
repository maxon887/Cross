#include "BillboardScene.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Entity.h"
#include "Mesh.h"
#include "Transform.h"

void BillboardScene::Start() {
	DemoScene::Start();

	Shader* shader = GetShader("Engine/Shaders/Texture.sha");
	cubeMaterial = CREATE Material(shader);
	cubeMaterial->SetPropertyValue("Texture", GetTexture("Textures/ContainerDiffuse.png"));

	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(cubeMaterial);
	AddEntity(cube);

	Shader* billboardShader = GetShader("Engine/Shaders/Billboard.sha");
	billboardMaterial = CREATE Material(billboardShader);
	billboardMaterial->SetPropertyValue("Texture", GetTexture("Textures/ContainerDiffuse.png"));
	Entity* plane = LoadPrimitive(Model::Primitive::PLANE);
	plane->GetComponent<Mesh>()->SetMaterial(billboardMaterial);
	plane->GetComponent<Transform>()->SetPosition(Vector3D(0.0f, 2.0f, 0.0));
	AddEntity(plane);
}

void BillboardScene::Stop() {
	delete cubeMaterial;
	delete billboardMaterial;
	DemoScene::Stop();
}

void BillboardScene::Update(float sec) {
	DemoScene::Update(sec);
}
