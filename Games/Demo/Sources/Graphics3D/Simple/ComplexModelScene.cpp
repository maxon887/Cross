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
#include "ComplexModelScene.h"
#include "GraphicsGL.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Model.h"
#include "Shaders/Shader.h"
#include "Material.h"

void ComplexModelScene::Start(){
	CCScene::Start();
	SetOrbitDistance(60.f);
	
	shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	shader->Compile();

	texture = gfx2D->LoadTexture("gfx3D/Camaro/Diffuse.png");
	material = new Material(shader);
	material->SetPropertyValue("Texture", (void*)texture->GetID());

	camaro = gfx3D->LoadModel("gfx3D/Camaro/camaro.fbx");
	camaro->SetMaterial(material);
}

void ComplexModelScene::Stop(){
	delete camaro;
	delete material;
	delete texture;
	delete shader;
	CCScene::Stop();
}

void ComplexModelScene::Update(float sec){
	camaro->Draw();
	CCScene::Update(sec);
}