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
#include "LightMapsScreen.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Model.h"
#include "Game.h"

void LightMapsScreen::Start(){
	CameraControlScreen::Start();

	light = new Light(0.5f, 1.f, 1.f);
	light->SetPosition(Vector3D(13.f, -3.f, -5.f));
	gfx3D->ClearLightSources();
	gfx3D->AddLightSource(light);
	model = gfx3D->LoadModel(Shader::Type::LIGHT_MAPS, "gfx3D/Cube.obj");
	Texture* diffuseTexture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse.png", Texture::Filter::TRILINEAR);
	Texture* specularTexture = gfx2D->LoadTexture("gfx3D/ContainerSpecular.png", Texture::Filter::TRILINEAR);
	model->SetDiffuseTexture(diffuseTexture);
	model->SetSpecularTexture(specularTexture);
}

void LightMapsScreen::Stop(){
	CameraControlScreen::Stop();
	delete model;
}

void LightMapsScreen::Update(float sec){
	light->Draw();
	model->Draw();
	model->SetRotateY(game->GetRunTime() * 15.f);

	CameraControlScreen::Update(sec);
}