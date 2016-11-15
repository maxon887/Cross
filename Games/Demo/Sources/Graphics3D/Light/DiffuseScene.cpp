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
#include "DiffuseScene.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Config.h"
#include "Camera.h"

void DiffuseScene::Start(){
	CameraControlsScreen::Start();
	//light setups
	Light* light = new Light(Light::POINT);
	light->SetPosition(Vector3D(2.f, 2.f, -2.f));
	AddLight(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular_diffuse.vert", "gfx3D/shaders/specular_diffuse.frag");
	shader->AddProperty("Diffuse Texture", "uDiffuseTexture");
	shader->AddProperty("Diffuse Color", "uColor");
	shader->AddProperty("Specular Color", "uSpecularColor");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse");
	material = new Material(shader);
	material->SetPropertyValue("Diffuse Texture", texture);
	material->SetPropertyValue("Diffuse Color", Color::White);
	material->SetPropertyValue("Specular Color", Color::White);
	material->SetPropertyValue("Shininess",	0.5f * 128.f);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetMaterial(material);
}

void DiffuseScene::Stop(){
	delete cube;
	delete material;
	delete texture;
	delete shader;
	CameraControlsScreen::Stop();
}

void DiffuseScene::Update(float sec){
	CameraControlsScreen::Update(sec);
	cube->Draw();
}