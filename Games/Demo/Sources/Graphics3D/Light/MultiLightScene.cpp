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
#include "MultiLightScene.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"

void MultiLightScene::Start(){
	CameraControlsScreen::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -30.f));
	
	for(U32 i = 0; i < 8; ++i){
		Light* light = new Light(Light::Type::POINT);
		light->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		light->SetColor(Color::Red);
		AddLight(light);
	}
	
	for(U32 i = 0; i < 1; ++i){
		Light* light = new Light(Light::Type::DIRECTIONAL);
		light->SetDirection(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)));
		light->SetColor(Color::Green);
		AddLight(light);
	}
	
	for(U32 i = 0; i < 4; ++i){
		Light* light = new Light(Light::Type::SPOT);
		light->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		light->LookAt(Vector3D(0.f));
		light->SetColor(Color::Blue);
		AddLight(light);
	}
	
	shader = gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	shader->AddMakro("USE_DIFFUSE_MAP");
	shader->AddMakro("USE_SPECULAR_MAP");
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
	
	for(U32 i = 0; i < 50; ++i){
		Model* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-10.f, 10.f), Random(-10.f, 10.f), Random(-10.f, 10.f)));
		clone->SetRotate(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)), Random(0.f, 360.f));
		objects.push_back(clone);
	}
}

void MultiLightScene::Stop(){
	for(Model* clone : objects){
		delete clone;
	}
	delete cube;
	delete material;
	delete diffuse_texture;
	delete specular_map;
	delete shader;
	CameraControlsScreen::Stop();
}

void MultiLightScene::Update(float sec){
	for(Model* obj : objects){
		obj->Draw();
	}
	CameraControlsScreen::Update(sec);
}