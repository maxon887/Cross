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
#include "SpecularScene.h"
#include "Light.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"
#include "Shaders/LightsShader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

void SpecularScene::Start() {
	DemoScene::Start();
	GetCamera()->GetTransform()->SetPosition(Vector3D(0.f, 0.f, -2.f));
	FreeCameraScene::LookAtCamera(Vector3D(0.f, 0.3f, 0.f));
	//lights
	light = new Entity("PointLight");
	light->AddComponent(new Transform());
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);

	shader = new LightsShader();
	shader->AddProperty("Transparency", "uTransparency", 1.f);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Specular Multiplier", "uSpecularMultiplier", 4.f);
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();

	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("Models/Camaro/Diffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("Models/Camaro/Specular.png"));
	material->SetPropertyValue("Shininess", 64.f);
	Entity* model = GetModel("Models/Camaro/Camaro.fbx")->GetHierarchy();
	ApplyMaterial(model, material);
	AddEntity(model);
}

void SpecularScene::Stop() {
	delete material;
	delete shader;
	DemoScene::Stop();
}

void SpecularScene::Update(float sec) {
	DemoScene::Update(sec);
	light->GetTransform()->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}