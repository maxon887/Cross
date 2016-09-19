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
#include "Utils/Light.h"
#include "Material.h"
#include "Model.h"

void MultiLightScene::Start(){
	CCScene::Start();
	
	SetOrbitDistance(60.f);
	DrawLights(true);

	for(int i = 0; i < 8; ++i){
		Light* light = new Light(Light::Type::POINT);
		light->SetPosition(Vector3D(Random(-30.f, 30.f), Random(-30.f, 30.f), Random(-30.f, 30.f)));
		light->SetColor(Color::Red);
		AddLight(light);
	}
	
	for(int i = 0; i < 1; ++i){
		Light* light = new Light(Light::Type::DIRECTIONAL);
		light->SetDirection(Vector3D(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f)));
		light->SetColor(Color::Green);
		AddLight(light);
	}
	
	for(int i = 0; i < 4; ++i){
		Light* light = new Light(Light::Type::SPOT);
		light->SetPosition(Vector3D(Random(-30.f, 30.f), Random(-30.f, 30.f), Random(-30.f, 30.f)));
		light->LookAt(Vector3D(0.f));
		light->SetColor(Color::Blue);
		AddLight(light);
	}

	Shader* shader = gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	material = new Material(shader);
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	material->SetPropertyValue("Diffuse Texture", (void*)diffuseTexture->GetID());
	material->SetPropertyValue("Specular Map", (void*)specularTexture->GetID());
	shininess = 0.5f * 128.f;
	material->SetPropertyValue("Shininess", (void*)(&shininess));
	cube = gfx3D->LoadModel("Engine/gfx3D/Cube.obj");
	cube->SetMaterial(material);

	for(int i = 0; i < 10; ++i){
		Model* clone = cube->Clone();
		clone->SetPosition(Vector3D(Random(-20.f, 20.f), Random(-20.f, 20.f), Random(-20.f, 20.f)));
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
	CCScene::Stop();
}

void MultiLightScene::Update(float sec){
	for(Model* obj : objects){
		obj->Draw();
	}
	CCScene::Update(sec);
}