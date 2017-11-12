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
#include "TexturedModelScene.h"
#include "Material.h"
#include "Entity.h"
#include "Texture.h"
#include "Mesh.h"

void TexturedModelScene::Start(){
	DemoScene::Start();
	material = new Material(GetShader("Engine/Shaders/Texture.sha"));
	material->SetPropertyValue("Texture", GetTexture("gfx3D/ContainerDiffuse.png"));
	Entity* cube = LoadPrimitive(Model::Primitive::CUBE);
	cube->GetComponent<Mesh>()->SetMaterial(material);
	AddEntity(cube);
}

void TexturedModelScene::Stop(){
	delete material;
	DemoScene::Stop();
}

void TexturedModelScene::Update(float sec){
	DemoScene::Update(sec);
}