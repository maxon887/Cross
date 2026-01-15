#include "NakedScene.h"
#include "Light.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"
#include "Shaders/MultiLightShader.h"
#include "Transform.h"
#include "Camera.h"
#include "CameraController.h"

#include <cmath>

void NakedScene::Start() {
	DemoScene::Start();
	GetCamera()->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -2.f));
	camera_controller->LookAtTarget(Vector3D(0.f, 0.3f, 0.f));
	
	//lights
	light = CREATE Entity("PointLight");
	light->AddComponent(CREATE Transform());
	light->AddComponent(CREATE Light(Light::Type::POINT));
	AddEntity(light);

	shader = CREATE MultiLightShader();
	shader->AddUniform("Transparency", "uTransparency", 1.f);
	shader->AddUniform("Diffuse Color", "uDiffuseColor");
	shader->AddUniform("Specular", "uSpecular");
	shader->AddUniform("Shininess", "uShininess");
	shader->Compile();
	material = CREATE Material(shader);
	material->SetPropertyValue("Diffuse Color", Color::White);
	material->SetPropertyValue("Specular", 2.f);
	material->SetPropertyValue("Shininess", 64.f);
	Entity* model = GetModel("Models/Camaro/Camaro.fbx")->GetHierarchy();
	ApplyMaterial(model, material);
	AddEntity(model);
}

void NakedScene::Stop() {
	delete material;
	delete shader;
	DemoScene::Stop();
}

void NakedScene::Update(float sec) {
	DemoScene::Update(sec);
	light->GetTransform()->SetPosition(Vector3D(cosf(game->GetRunTime() / 2.f)*3.f, 2.f, sinf(game->GetRunTime() / 2.f)*3.f));
}
