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

#include "Game.h"
#include "Graphics3D.h"
#include "Graphics2D.h"
#include "Model.h"
#include "Material.h"

void TexturedModelScene::Start() {
	CCScene::Start();
	Shader* shader = gfxGL->GetShader(DefaultShader::TEXTURE);
	material = new Material(shader);
	texture = gfx2D->LoadTexture("gfx3D/Box.png", Texture::Filter::TRILINEAR);
	material->SetDiffuseTexture(texture);
	cube = gfx3D->LoadMesh("gfx3D/Cube.obj");
	cube->SetMaterial(material);
}

void TexturedModelScene::Stop() {
	delete cube;
	delete material;
	delete texture;
	CCScene::Stop();
}

void TexturedModelScene::Update(float sec) {
	cube->Draw();
	CCScene::Update(sec);
}