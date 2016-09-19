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
#include "SpecularScene.h"
#include "Game.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/LightShader.h"

void SpecularScene::Start(){
	CCScene::Start();
	DrawLights(false);
	SetCameraViewDistance(500.f);
	SetOrbitDistance(28.f);
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	//light setups
	Light* light = new Light(Light::Type::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddLight(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	shader->AddProperty("Diffuse Color", Shader::Property::Type::VEC3, "uColor");
	shader->AddProperty("Specular Color", Shader::Property::Type::VEC3, "uSpecularColor");
	shader->AddProperty("Shininess", Shader::Property::Type::FLOAT, "uShininess");
	shader->Compile();
	material = new Material(shader);
	material->SetPropertyValue("Diffuse Color", (void*)&Color::Red);
	material->SetPropertyValue("Specular Color", (void*)&Color::White);
	shininess = 0.5f * 128.f;
	material->SetPropertyValue("Shininess", (void*)(&shininess));
	cube = gfx3D->LoadModel("Engine/gfx3D/Cube.obj");
	cube->SetMaterial(material);

	cube->SetPosition(Vector3D(1.0f, 2.0f, -1.0f));
	cube->SetScale(Vector3D(0.73f, 1.0f, 0.55f));
}

void SpecularScene::Stop(){
	delete cube;
	delete material;
	delete shader;
	CCScene::Stop();
}

void SpecularScene::Update(float sec){
	cube->Draw();
	cube->SetRotateY(game->GetRunTime() * 15.f);
	CCScene::Update(sec);
}
