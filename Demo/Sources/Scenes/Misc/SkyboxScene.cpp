#include "SkyboxScene.h"
#include "Utils/Cubemap.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Entity.h"
#include "Mesh.h"

void SkyboxScene::Start() {
	DemoScene::Start();
	
	Cubemap* cubemap = CREATE Cubemap( "Textures/Skybox/right.png",
									"Textures/Skybox/left.png",
									"Textures/Skybox/up.png",
									"Textures/Skybox/down.png",
									"Textures/Skybox/back.png",
									"Textures/Skybox/front.png" );
	skybox = CREATE Skybox(cubemap);

	Shader* shader = GetShader("Engine/Shaders/Texture.sha");
	material = CREATE Material(shader);
	Texture* texture = GetTexture("Textures/ContainerDiffuse.png");
	material->SetPropertyValue("Texture", texture);
	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	AddEntity(cube);
}

void SkyboxScene::Stop() {
	delete skybox;
	delete material;
	DemoScene::Stop();
}

void SkyboxScene::Update(float sec) {
	DemoScene::Update(sec);
	skybox->Draw();
}