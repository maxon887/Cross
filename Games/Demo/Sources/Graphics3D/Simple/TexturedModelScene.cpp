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
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Material.h"
#include "Model.h"

void TexturedModelScene::Start(){
	CameraControlsScreen::Start();
	shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	shader->Compile();
	material = new Material(shader);
	texture = gfx2D->LoadTexture("gfx3D/ContainerDiffuse");
	material->SetPropertyValue("Texture", (void*)texture->GetID());
	cube = gfx3D->LoadPrimitive(Graphics3D::Primitives::CUBE);
	cube->SetMaterial(material);
}

void TexturedModelScene::Stop(){
	delete cube;
	delete material;
	delete texture;
	delete shader;
	CameraControlsScreen::Stop();
}

void TexturedModelScene::Update(float sec){
	cube->Draw();
	CameraControlsScreen::Update(sec);
}