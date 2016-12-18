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
#include "PBRScene.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Material.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Game.h"

void PBRScene::Start(){
	CameraControlsScreen::Start();
	glClearColor(0.15f, 0.15f, 0.15f, 1.f);
	embient = new Cubemap( "gfx3D/Skybox/right.png",
									"gfx3D/Skybox/left.png",
									"gfx3D/Skybox/up.png",
									"gfx3D/Skybox/down.png",
									"gfx3D/Skybox/back.png",
									"gfx3D/Skybox/front.png");

	skybox = new Skybox(embient);
	light = new Light(Light::Type::POINT);
	AddLight(light);

	albedo = gfx2D->LoadTexture("gfx3D/Revolver/Diffuse.png", Texture::TilingMode::REPEAT);
	metalness = gfx2D->LoadTexture("gfx3D/Revolver/Metallic.png", Texture::TilingMode::REPEAT);
	roughness = gfx2D->LoadTexture("gfx3D/Revolver/Roughness.png", Texture::TilingMode::REPEAT);
	normals = gfx2D->LoadTexture("gfx3D/Revolver/Normal.png", Texture::TilingMode::REPEAT);
	model = gfx3D->LoadModel("gfx3D/Revolver/Revolver.fbx");

	shader = new LightShader("gfx3D/shaders/PBR.vert", "gfx3D/shaders/PBR3.frag");
	shader->AddProperty("Albedo Map", "uAlbedoMap");
	shader->AddProperty("Metalness Map", "uMetallmessMap");
	shader->AddProperty("Roughness Map", "uRoughnessMap");
	shader->Compile();
	
	material = new Material(shader);
	material->SetPropertyValue("Albedo Map", albedo);
	material->SetPropertyValue("Metalness Map", metalness);
	material->SetPropertyValue("Roughness Map", roughness);

	model->SetMaterial(material);
	model->SetScale(0.01);
}

void PBRScene::Stop(){

	CameraControlsScreen::Stop();
}

void PBRScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	//skybox->Draw();
	model->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*3.f, 2.f, sin(game->GetRunTime() / 2.f)*3.f));
}