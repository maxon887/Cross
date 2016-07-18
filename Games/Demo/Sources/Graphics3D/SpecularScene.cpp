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
#include "SpecularScene.h"
#include "Mesh.h"
#include "Demo.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Sprite.h"
#include "Material.h"
#include "Graphics3D/Shaders/LightShader.h"
#include "Graphics2D.h"
#include "Utils/Light.h"

void SpecularScene::Start(){
	CCScene::Start();
	
	SetOrbitDistance(28.f);
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	//light setups
	Light* light = new Light(Light::Type::POINT);
	light->SetPosition(Vector3D(10.f, 7.f, -5.f));
	AddLight(light);
	//scene setups
	shader = new LightShader("gfx3D/shaders/specular.vert", "gfx3D/shaders/specular.frag");
	material = new Material(shader);
	material->SetDiffuseColor(Color::Red);
	cube = gfx3D->LoadMesh("gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void SpecularScene::Stop(){
	delete cube;
	delete material;
	delete shader;
	CCScene::Stop();
}

void SpecularScene::Update(float sec){
	cube->Draw();
	CCScene::Update(sec);
}
