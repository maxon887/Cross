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
#include "DebugScene.h"
#include "Graphics3D.h"
#include "Shaders/Shader.h"
#include "Material.h"
#include "Model.h"
#include "Light.h"

using namespace cross;

DebugScene::DebugScene() :
	point_light(NULL),
	spot_light(NULL),
	light_material(NULL) 
{ }

void DebugScene::Start(){
	FreeCameraScene::Start();
	//light drawing stuff
	light_shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	light_shader->Compile();
	light_material = new Material(light_shader);
	point_light = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	point_light->SetScale(0.2f);
	point_light->SetMaterial(light_material);
	spot_light = gfx3D->LoadPrimitive(Graphics3D::Primitives::SPHERE);
	spot_light->SetScale(0.2f);
	spot_light->SetMaterial(light_material);
}

void DebugScene::Update(float sec){
	FreeCameraScene::Update(sec);
	DrawLights();
}

void DebugScene::Stop(){
	delete point_light;
	delete spot_light;
	delete light_material;
	delete light_shader;
	FreeCameraScene::Stop();
}

void DebugScene::DrawLights(){
	for(Light* light : lights){
		switch(light->GetType()) {
		case Light::Type::POINT:
			point_light->SetPosition(light->GetPosition());
			point_light->GetMaterial()->SetPropertyValue("Color", light->GetColor());
			point_light->Draw();
		break;
		case Light::Type::SPOT:
			spot_light->SetPosition(light->GetPosition());
			spot_light->SetDirection(light->GetDirection());
			spot_light->GetMaterial()->SetPropertyValue("Color", light->GetColor());
			spot_light->Draw();
		break;
		default:
			break;
		}
	}
}