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
#include "SolidModelScene.h"
#include "Material.h"
#include "Mesh.h"
#include "Entity.h"

void SolidModelScene::Start(){
	CameraControlsScene::Start();
	Shader* shader = GetShader("Engine/Shaders/Simple.sha");
	material = new Material(shader);
	material->SetPropertyValue("Color", Color::Green);
	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	AddEntity(cube);
}

void SolidModelScene::Stop(){
	delete material;
	CameraControlsScene::Stop();
}

void SolidModelScene::Update(float sec){
	CameraControlsScene::Update(sec);
}