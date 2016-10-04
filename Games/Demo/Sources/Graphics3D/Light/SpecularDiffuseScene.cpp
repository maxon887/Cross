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
#include "SpecularDiffuseScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Config.h"
#include "Camera.h"

void SpecularDiffuseScene::Start(){
	FreeCameraScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	//light setups
	Light* light = new Light(Light::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddLight(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular_diffuse.vert", "gfx3D/shaders/specular_diffuse.frag");
	shader->AddProperty("Diffuse Texture", Shader::Property::SAMPLER, "uDiffuseTexture");
	shader->AddProperty("Diffuse Color", Shader::Property::Type::VEC3, "uColor");
	shader->AddProperty("Shininess", Shader::Property::FLOAT, "uShininess");
	shader->Compile();
	texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse");
	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", (void*)texture->GetID());
	material->SetPropertyValue("Diffuse Color", (void*)&Color::White);
	shininess = 0.5f * 128.f;
	material->SetPropertyValue("Shininess",	(void*)(&shininess));
	cube = gfx3D->LoadModel("Engine/gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void SpecularDiffuseScene::Stop(){
	delete cube;
	delete material;
	delete texture;
	delete shader;
	FreeCameraScene::Stop();
}

void SpecularDiffuseScene::Update(float sec){
	cube->Draw();
	FreeCameraScene::Update(sec);
}