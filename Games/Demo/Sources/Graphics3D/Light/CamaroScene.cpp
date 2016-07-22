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
	SetAmbientColor(Color(0.05f, 0.05f, 0.05f));
	SetCameraViewDistance(500.f);
	SetBackground(Color::Black);
	SetOrbitDistance(60.f);

	shader = new MultiLightShader("gfx3D/Camaro/camaro.vert", "gfx3D/Camaro/camaro.frag");
	camaro = gfx3D->LoadModel("gfx3D/Camaro/camaro.3DS");
	camaro->SetShader(shader);
	camaro->SetRotateX(-90.f);
	
	road = gfx3D->LoadModel("gfx3D/Road/road.3DS");
	road->SetShader(gfxGL->GetShader(DefaultShader::MULTI_LIGHT));
	road->GetMaterial(0)->GetDiffuseTexture()->SetTilingMode(Texture::TilingMode::REPEAT);
	road->SetRotateX(-90.f);
	//lights
	light = new Light(Light::Type::POINT);
	AddLight(light);
}

void CamaroScene::Stop(){
	delete road;
	delete camaro;
	delete shader;
	CCScene::Stop();
}

void CamaroScene::Update(float sec){
	camaro->Draw();
	road->Draw();
	light->SetPosition(Vector3D(cos(game->GetRunTime() / 2.f)*23.f, 12.f, sin(game->GetRunTime() / 2.f)*23.f));
	CCScene::Update(sec);
}