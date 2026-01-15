#include "RoughnessScene.h"
#include "Light.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"
#include "Shaders/MultiLightShader.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraController.h"

#include <cmath>

void RoughnessScene::Start(){
	DemoScene::Start();
	GetCamera()->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -2.f));
	camera_controller->LookAtTarget(Vector3D(0.f, 0.3f, 0.f));
	//lights
	light = CREATE Entity("Point Light");
	light->AddComponent(CREATE Transform());
	light->AddComponent(CREATE Light(Light::Type::POINT));
	AddEntity(light);

	shader = CREATE MultiLightShader();
	shader->AddUniform("Transparency", "uTransparency", 1.f);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
	shader->AddMacro("USE_SHININESS_MAP");
	shader->AddUniform("Diffuse Texture", "uDiffuseTexture");
	shader->AddUniform("Specular Map", "uSpecularMap");
	shader->AddUniform("Specular Multiplier", "uSpecularMultiplier", 4.f);
	shader->AddUniform("Shininess Map", "uShininessMap");
	shader->AddUniform("Shininess Multiplier", "uShininessMultiplier", 64.f);
	shader->Compile();

	material = CREATE Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("Models/Camaro/Diffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("Models/Camaro/Specular.png"));
	material->SetPropertyValue("Shininess Map", GetTexture("Models/Camaro/Shininess.png"));
	Entity* model = GetModel("Models/Camaro/Camaro.fbx")->GetHierarchy();
	ApplyMaterial(model, material);
	AddEntity(model);
}

void RoughnessScene::Stop(){
	delete material;
	delete shader;
	DemoScene::Stop();
}

void RoughnessScene::Update(float sec){
	DemoScene::Update(sec);
	light->GetTransform()->SetPosition(Vector3D(cosf(game->GetRunTime() / 2.f)*3.f, 2.f, sinf(game->GetRunTime() / 2.f)*3.f));
}