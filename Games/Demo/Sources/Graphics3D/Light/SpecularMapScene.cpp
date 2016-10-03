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
#include "SpecularMapScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"

void SpecularMapScene::Start() {
	FreeCameraScene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	//light setups
	Light* light = new Light(Light::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddLight(light);

	shader = new LightShader("gfx3D/shaders/specular_map.vert", "gfx3D/shaders/specular_map.frag");
	shader->AddProperty("Diffuse Texture", Shader::Property::Type::SAMPLER, "uDiffuseTexture");
	shader->AddProperty("Specular Map", Shader::Property::Type::SAMPLER, "uSpecularTexture");
	shader->AddProperty("Shininess", Shader::Property::Type::FLOAT, "uShininess");
	shader->Compile();
	material = new Material(shader);
	diffuse_texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse");
	specular_map = gfx2D->LoadTexture("gfx3D/ContainerSpecular");
	material->SetPropertyValue("Diffuse Texture", (void*)diffuse_texture->GetID());
	material->SetPropertyValue("Specular Map", (void*)specular_map->GetID());
	shininess = 0.5f * 128.f;
	material->SetPropertyValue("Shininess", (void*)(&shininess));
	cube = gfx3D->LoadModel("Engine/gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void SpecularMapScene::Stop() {
	delete cube;
	delete material;
	delete diffuse_texture;
	delete specular_map;
	delete shader;
	FreeCameraScene::Stop();
}

void SpecularMapScene::Update(float sec) {
	cube->Draw();
	DrawLights();
	FreeCameraScene::Update(sec);
}