/*	Copyright © 2018 Maksim Lukyanov

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
#include "NakedScene.h"
#include "Light.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"
#include "Shaders/LightsShader.h"
#include "Transform.h"
#include "System.h"
#include "Camera.h"

void NakedScene::Start() {
	DemoScene::Start();
	GetCamera()->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -2.f));
	FreeCameraScene::LookAtTarget(Vector3D(0.f, 0.3f, 0.f));
	
	//lights
	light = new Entity("PointLight");
	light->AddComponent(new Transform());
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);

	shader = new LightsShader();
	shader->AddProperty("Transparency", "uTransparency", 1.f);
	shader->AddProperty("Diffuse Color", "uDiffuseColor");
	shader->AddProperty("Specular", "uSpecular");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	material = new Material(shader);
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
	light->GetTransform()->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}
