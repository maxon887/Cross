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
#include "StencilScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Config.h"
#include "Camera.h"

void StencilScene::Start(){
	CameraControlsScreen::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -6.f));
	//light setups
	Light* light = new Light(Light::POINT);
	light->SetPosition(Vector3D(2.f, 2.f, -2.f));
	AddLight(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular_diffuse.vert", "gfx3D/shaders/specular_diffuse.frag");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Diffuse Color", "uColor");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	cube_texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse");
	cube_mat = new Material(shader);
	cube_mat->SetPropertyValue("Diffuse Texture", cube_texture);
	cube_mat->SetPropertyValue("Diffuse Color", Color::White);
	cube_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetMaterial(cube_mat);
	cube->SetPosition(Vector3D(0.f, 0.5f, 0.f));
	road_texture = gfx2D->LoadTexture("gfx3D/Road/Diffuse");
	road_texture->SetTilingMode(Texture::TilingMode::REPEAT);
	road_mat = new Material(shader);
	road_mat->SetPropertyValue("Diffuse Texture", road_texture);
	road_mat->SetPropertyValue("Diffuse Color", Color::White);
	road_mat->SetPropertyValue("Shininess", 0.5f * 128.f);
	road = gfx3D->LoadModel("gfx3D/Road/road.3DS");
	road->SetRotateX(-90.f);
	road->SetScale(.2f);
	road->SetMaterial(road_mat);
	road->FaceCulling(false);
}

void StencilScene::Stop(){
	delete cube;
	delete road;
	delete cube_mat;
	delete road_mat;
	delete cube_texture;
	delete road_texture;
	delete shader;
	CameraControlsScreen::Stop();
}

void StencilScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	cube->Draw();
	road->Draw();
}