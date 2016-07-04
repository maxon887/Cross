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
#include "Demo.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Sprite.h"
#include "Material.h"
#include "Graphics3D/Shaders/SpecularShader.h"

void MaterialScene::Start(){
	CCScene::Start();
	
	SetOrbitDistance(28.f);
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	//light setups
	PointLight* light = new PointLight(Color::White);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddPointLight(light);
	//scene setups
	SpecularShader* shader = new SpecularShader();
	Material* material = new Material(shader);
	material->SetDiffuseColor(Color::Red);
	cube = gfx3D->LoadMesh("gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void MaterialScene::Stop(){
	CCScene::Stop();
}

void MaterialScene::Update(float sec){
	cube->Draw();
	CCScene::Update(sec);
}
