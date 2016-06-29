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
#include "DiffuseMapScreen.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Model.h"
#include "Game.h"

void DiffuseMapScreen::Start(){
	CCScene::Start();

	light_caster_mesh = gfx3D->LoadMesh("gfx3D/Cube.obj");
	light = new LightCaster(light_caster_mesh, Vector3D(0.5f), Vector3D(1.f), Vector3D(1.f));
	light->SetPosition(Vector3D(13.f, -3.f, -5.f));
	light->SetScale(0.2f);
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/Box.png", Texture::Filter::TRILINEAR);
	//model = gfx3D->LoadModel("gfx3D/Cube.obj", diffuseTexture);
}

void DiffuseMapScreen::Stop(){
	CCScene::Stop();
	delete model;
	delete light;
}

void DiffuseMapScreen::Update(float sec){/*
	light->Draw();
	gfx3D->DrawModelLightDiffuse(model, light);

	model->SetRotateY(game->GetRunTime() * 15.f);
	*/
	CCScene::Update(sec);
}