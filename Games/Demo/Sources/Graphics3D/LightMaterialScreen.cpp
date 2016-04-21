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
#include "LightMaterialScreen.h"
#include "Demo.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Sprite.h"

void LightMaterialScreen::Start(){
	CameraControlScreen::Start();
	current_model = 0;
	gfx3D->GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	orbit_distance = 28.f;
	Model* cube = gfx3D->LoadModel(Shader::LIGHT_MATERIAL, "gfx3D/Cube.obj");
	models.push_back(cube);
	Model* sphere = gfx3D->LoadModel(Shader::LIGHT_MATERIAL, "gfx3D/Sphere.obj");
	sphere->SetColor(Color::Red);
	models.push_back(sphere);
	Model* gnome = gfx3D->LoadModel(Shader::LIGHT_MATERIAL, "gfx3D/Gnome.obj");
	gnome->SetColor(Color::Green);
	gnome->SetScale(2.f);
	models.push_back(gnome);

	light = new Light();
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	light->SetAmbientStrength(1.f);
	light->SetDiffuseStrength(1.f);
	light->SetSpecularStrength(1.f);
	gfx3D->ClearLightSources();
	gfx3D->AddLightSource(light);

	Sprite* arrowUp = demo->GetCommonSprite("ArrowUp.png");
	Sprite* arrowDown = demo->GetCommonSprite("ArrowDown.png");
	arrowUp->SetScale(Vector2D(0.5f, 2.f));
	arrowDown->SetScale(Vector2D(0.5f, 2.f));
	Vector2D pos;
	pos.x = GetWidth() - arrowUp->GetWidth() / 2.f - 10.f;
	pos.y = GetHeight() / 2.f;
	next_model = new Button(pos);
	next_model->SetImages(arrowUp->Clone(), arrowDown->Clone());
	next_model->Clicked += MakeDelegate(this, &LightMaterialScreen::NextModelClick);
	pos.x = arrowUp->GetWidth() / 2.f + 10.f;
	prev_model = new Button(pos);
	arrowUp->SetRotate(180.f);
	arrowDown->SetRotate(180.f);
	prev_model->SetImages(arrowUp, arrowDown);
	prev_model->Clicked += MakeDelegate(this, &LightMaterialScreen::PrevModelClick);
}

void LightMaterialScreen::Stop(){
	CameraControlScreen::Stop();
	delete light;
	delete next_model;
	delete prev_model;
	for(Model* model : models){
		delete model;
	}
}

void LightMaterialScreen::Update(float sec){
	light->Draw();

	models[current_model]->Draw();
	models[current_model]->SetRotateY(game->GetRunTime() * 15.f);

	next_model->Update();
	prev_model->Update();

	CameraControlScreen::Update(sec);
}

void LightMaterialScreen::NextModelClick(){
	current_model++;
	current_model = current_model % models.size();
}

void LightMaterialScreen::PrevModelClick(){
	if(current_model == 0){
		current_model = models.size();
	}
	current_model--;
	current_model = current_model % models.size();
}