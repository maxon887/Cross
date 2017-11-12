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
#include "RoughnessScene.h"
#include "Light.h"
#include "GraphicsGL.h"
#include "Material.h"
#include "Game.h"
#include "Entity.h"
#include "Shaders/LightsShader.h"
#include "Texture.h"
#include "Transform.h"

void RoughnessScene::Start(){
	DemoScene::Start();
	//lights
	light = new Entity("Point Light");
	light->AddComponent(new Transform());
	light->AddComponent(new Light(Light::Type::POINT));
	AddEntity(light);

	shader = new LightsShader();
	shader->AddProperty("Transparency", "uTransparency", 1.f);
	shader->AddMacro("USE_DIFFUSE_MAP");
	shader->AddMacro("USE_SPECULAR_MAP");
	shader->AddMacro("USE_SHININESS_MAP");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Specular Multiplier", "uSpecularMultiplier", 4.f);
	shader->AddProperty("Shininess Map", "uShininessMap");
	shader->AddProperty("Shininess Multiplier", "uShininessMultiplier", 64.f);
	shader->Compile();

	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", GetTexture("gfx3D/Camaro/Diffuse.png"));
	material->SetPropertyValue("Specular Map", GetTexture("gfx3D/Camaro/Specular.png"));
	material->SetPropertyValue("Shininess Map", GetTexture("gfx3D/Camaro/Shininess.png"));
	Entity* model = GetModel("gfx3D/Camaro/Camaro.fbx")->GetHierarchy();
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
	light->GetTransform()->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}