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
#include "MaterialScreen.h"
#include "Demo.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Sprite.h"

void MaterialScreen::Start(){
	CCScene::Start();
	/*
	current_model = 0;
	gfx3D->GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	orbit_distance = 28.f;
	Model* cube = gfx3D->LoadModel("gfx3D/Cube.obj", Material::Bronze);
	models.push_back(cube);
	Model* sphere = gfx3D->LoadModel("gfx3D/Sphere.obj", Material::Bronze);
	models.push_back(sphere);
	Model* gnome = gfx3D->LoadModel("gfx3D/Gnome.obj", Material::Bronze);
	gnome->SetScale(2.f);
	models.push_back(gnome);

	light_caster_mesh = gfx3D->LoadMesh("Gfx3D/Cube.obj");
	light = new LightCaster(light_caster_mesh, Vector3D(0.2f), Vector3D(1.f), Vector3D(0.5f));
	light->SetScale(0.2f);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	light->SetAmbientStrength(1.f);
	light->SetDiffuseStrength(1.f);
	light->SetSpecularStrength(1.f);

	Sprite* arrowUp = demo->GetCommonSprite("ArrowUp.png");
	Sprite* arrowDown = demo->GetCommonSprite("ArrowDown.png");
	arrowUp->SetScale(Vector2D(0.5f, 2.f));
	arrowDown->SetScale(Vector2D(0.5f, 2.f));
	Vector2D pos;
	pos.x = GetWidth() - arrowUp->GetWidth() / 2.f - 10.f;
	pos.y = GetHeight() / 2.f;
	next_model = new Button(pos);
	next_model->SetImages(arrowUp->Clone(), arrowDown->Clone());
	next_model->Clicked += MakeDelegate(this, &MaterialScreen::NextModelClick);
	pos.x = arrowUp->GetWidth() / 2.f + 10.f;
	prev_model = new Button(pos);
	arrowUp->SetRotate(180.f);
	arrowDown->SetRotate(180.f);
	prev_model->SetImages(arrowUp, arrowDown);
	prev_model->Clicked += MakeDelegate(this, &MaterialScreen::PrevModelClick);*/
}

void MaterialScreen::Stop(){
	CCScene::Stop();
	delete light;
	delete next_model;
	delete prev_model;
	for(Model* model : models){
		delete model;
	}
}

void MaterialScreen::Update(float sec){/*
	light->Draw();

	gfx3D->DrawModelLightMaterial(models[current_model], light);

	models[current_model]->SetRotateY(game->GetRunTime() * 15.f);

	next_model->Update();
	prev_model->Update();
	*/
	CCScene::Update(sec);
}

void MaterialScreen::NextModelClick(){
	current_model++;
	current_model = current_model % models.size();
}

void MaterialScreen::PrevModelClick(){
	if(current_model == 0){
		current_model = models.size();
	}
	current_model--;
	current_model = current_model % models.size();
}