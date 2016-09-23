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
#include "CamaroScene.h"
#include "GraphicsGL.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Utils/Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

#include <math.h>

void CamaroScene::Start(){
	CCScene::Start();

	DrawLights(true);
	SetAmbientColor(Color(0.1f));
	SetCameraViewDistance(500.f);
	SetBackground(Color::Black);
	SetOrbitDistance(60.f);

	//lights
	light = new Light(Light::Type::POINT);
	AddLight(light);

	specular = 0.5f;
	shininess = 0.5f * 128.f;

	shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	shader->AddProperty("Diffuse Texture", Shader::Property::Type::SAMPLER, "uDiffuseTexture");
	shader->AddProperty("Specular", Shader::Property::Type::FLOAT, "uSpecular");
	shader->AddProperty("Shininess", Shader::Property::Type::FLOAT, "uShininess");
	shader->Compile();

	car_diffuse = gfx2D->LoadETC1Texture("gfx3D/Camaro/diffuse.pkm");
	car_mat = new Material(shader);
	car_mat->SetPropertyValue("Diffuse Texture", (void*)car_diffuse->GetID());
	car_mat->SetPropertyValue("Specular", (void*)(&specular));
	car_mat->SetPropertyValue("Shininess", (void*)(&shininess));
	camaro = gfx3D->LoadModel("gfx3D/Camaro/Camaro.fbx");
	camaro->SetMaterial(car_mat);
	
	road_diffuse = gfx2D->LoadETC1Texture("gfx3D/Road/diffuse.pkm");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(shader);
	road_mat->SetPropertyValue("Diffuse Texture", (void*)road_diffuse->GetID());
	road_mat->SetPropertyValue("Specular", (void*)(&specular));
	road_mat->SetPropertyValue("Shininess", (void*)(&shininess));
	road = gfx3D->LoadModel("gfx3D/Road/road.3DS");
	road->SetMaterial(road_mat);
	road->SetRotateX(-90.f);//du to 3ds exports
}

void CamaroScene::Stop(){
	delete road;
	delete camaro;
	delete car_mat;
	delete road_mat;
	delete car_diffuse;
	delete road_diffuse;
	delete shader;
	CCScene::Stop();
}

void CamaroScene::Update(float sec){
	camaro->Draw();
	road->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*23.f, 12.f, sin(game->GetRunTime() / 2.f)*23.f));
	CCScene::Update(sec);
}