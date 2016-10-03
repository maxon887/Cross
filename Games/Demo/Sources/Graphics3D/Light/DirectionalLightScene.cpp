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
#include "DirectionalLightScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"

void DirectionalLightScene::Start(){
	FreeCameraScene::Start();

	Light* light = new Light(Light::DIRECTIONAL);
	light->SetDirection(Vector3D(0.f, 0.f, 1.f));
	AddLight(light);

	shader = new LightShader("gfx3D/shaders/directional_light.vert", "gfx3D/shaders/directional_light.frag");
	shader->AddProperty("Diffuse Texture", Shader::Property::SAMPLER, "uDiffuseTexture");
	shader->AddProperty("Specular Map", Shader::Property::SAMPLER, "uSpecularMap");
	shader->AddProperty("Shininess", Shader::Property::FLOAT, "uShininess");
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

	for(U32 i = 0; i < 10; ++i){
		Model* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void DirectionalLightScene::Stop(){
	for(Model* clone : objects){
		delete clone;
	}
	delete cube;
	delete material;
	delete diffuse_texture;
	delete specular_map;
	delete shader;
	FreeCameraScene::Stop();
}

void DirectionalLightScene::Update(float sec){
	for(Model* obj : objects){
		obj->Draw();
	}
	FreeCameraScene::Update(sec);
}