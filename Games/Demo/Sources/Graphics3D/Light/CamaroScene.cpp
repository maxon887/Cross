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
#include "Light.h"
#include "Shaders/MultiLightShader.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"

#include <math.h>

void CamaroScene::Start(){
	CameraControlsScreen::Start();

	SetAmbientColor(Color(0.1f));

	//lights
	light = new Light(Light::Type::POINT);
	AddLight(light);

	shader = (MultiLightShader*)gfxGL->GetShader(DefaultShader::MULTI_LIGHT);
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Specular", "uSpecular");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();

	car_diffuse = gfx2D->LoadTexture("gfx3D/Camaro/Diffuse");
	car_mat = new Material(shader);
	car_mat->SetPropertyValue("Diffuse Texture", car_diffuse);
	car_mat->SetPropertyValue("Specular", 0.5f);
	car_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	camaro = gfx3D->LoadModel("gfx3D/Camaro/Camaro.fbx");
	camaro->SetMaterial(car_mat);
	
	road_diffuse = gfx2D->LoadTexture("gfx3D/RoadDiffuse");
	road_diffuse->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_diffuse);
	road_mat->SetPropertyValue("Specular", 0.5f);
	road_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	road_mat->SetPropertyValue("Tilling Factor", 3.f);
	road = gfx3D->LoadPrimitive(Graphics3D::Primitives::PLANE);
	road->SetScale(250.f);
	road->FaceCulling(false);
	road->SetMaterial(road_mat);
}

void CamaroScene::Stop(){
	delete road;
	delete camaro;
	delete car_mat;
	delete road_mat;
	delete car_diffuse;
	delete road_diffuse;
	delete shader;
	CameraControlsScreen::Stop();
}

void CamaroScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	camaro->Draw();
	road->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*23.f, 12.f, sin(game->GetRunTime() / 2.f)*23.f));
}