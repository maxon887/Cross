#include "BillboardScene.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Entity.h"
#include "Mesh.h"

void BillboardScene::Start() {
	DemoScene::Start();

	Shader* shader = GetShader("Engine/Shaders/Texture.sha");
	material = CREATE Material(shader);
	material->SetPropertyValue("Texture", GetTexture("Textures/ContainerDiffuse.png"));

	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	AddEntity(cube);
}

void BillboardScene::Stop() {
	delete material;
	DemoScene::Stop();
}

void BillboardScene::Update(float sec) {
	DemoScene::Update(sec);
}
