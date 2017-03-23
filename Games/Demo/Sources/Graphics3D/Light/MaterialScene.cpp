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
#include "MaterialScene.h"
#include "Game.h"
#include "Graphics3D.h"
#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Shaders/LightShader.h"
#include "Camera.h"

void MaterialScene::Start(){
	CameraControlsScene::Start();
	//light setups
	Entity* light = new Entity();
	light->AddComponent(new Light(Light::Type::POINT));
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddEntity(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	shader->AddProperty("Diffuse Color", "uColor");
	shader->AddProperty("Specular Color", "uSpecularColor");
	shader->AddProperty("Shininess", "uShininess");
	shader->Compile();
	material = new Material(shader);
	material->SetPropertyValue("Diffuse Color", Color::Red);
	material->SetPropertyValue("Specular Color", Color::White);
	material->SetPropertyValue("Shininess", 0.5f * 128.f);
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	gfx3D->AdjustMaterial(cube, material);
	AddEntity(cube);

	cube->SetScale(Vector3D(0.73f, 1.0f, 0.55f));
}

void MaterialScene::Stop(){
	CameraControlsScene::Stop();
}

void MaterialScene::Update(float sec){
	cube->SetRotateY(game->GetRunTime() * 15.f);
	CameraControlsScene::Update(sec);
}
