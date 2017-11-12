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
#include "DepthScene.h"
#include "GraphicsGL.h"
#include "Entity.h"
#include "Light.h"
#include "Game.h"
#include "Material.h"
#include "Texture.h"
#include "Config.h"
#include "Mesh.h"
#include "Transform.h"

void DepthScene::Start(){
	DemoScene::Start();

	SetAmbientColor(Color(0.1f));

	shader = new Shader("gfx3D/shaders/depth_test.vert", "gfx3D/shaders/depth_test.frag");
	shader->AddProperty("uFar", 30.f);
	shader->AddProperty("uNear", 0.1f);
	shader->Compile();

	car_mat = new Material(shader);
	Entity* camaro = GetModel("gfx3D/Camaro/Camaro.fbx")->GetHierarchy();
	ApplyMaterial(camaro, car_mat);
	AddEntity(camaro);
	
	road_mat = new Material(shader);
	Entity* road = LoadPrimitive(Model::Primitive::PLANE);
	road->GetTransform()->SetScale(15.f);
	road->GetComponent<Mesh>()->SetMaterial(road_mat);
	AddEntity(road);
}

void DepthScene::Stop(){
	delete road_mat;
	delete car_mat;
	delete shader;
	DemoScene::Stop();
}

void DepthScene::Update(float sec){
	DemoScene::Update(sec);
}