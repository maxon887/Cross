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
#include "SpotLightScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Camera.h"
	
void SpotLightScene::Start(){
	CameraControlsScene::Start();

	GetCamera()->SetPosition(Vector3D(-8.f, 0.f, -15.f));

	Light* light = new Light(Light::Type::SPOT);
	light->SetPosition(Vector3D(0.f, 0.f, -8.f));
	light->LookAt(Vector3D(0.f));
	light->SetCutOff(20.f);
	AddLight(light);

	shader = new LightShader("gfx3D/shaders/spot_light.vert", "gfx3D/shaders/spot_light.frag");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular Map", "uSpecularMap");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	material = new Material(shader);
	diffuse_texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png");
	specular_map = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png");
	material->SetPropertyValue("Diffuse Texture", diffuse_texture);
	material->SetPropertyValue("Specular Map", specular_map);
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetMaterial(material);

	for(U32 i = 0; i < 40; ++i){
		Model* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-5.f, 5.f), Random(-5.f, 5.f), Random(-5.f, 5.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void SpotLightScene::Stop(){
	for(Model* clone : objects){
		delete clone;
	}
	delete cube;
	delete material;
	delete diffuse_texture;
	delete specular_map;
	delete shader;
	CameraControlsScene::Stop();
}

void SpotLightScene::Update(float sec){
	for(Model* obj : objects){
		obj->Draw();
	}
	CameraControlsScene::Update(sec);
}