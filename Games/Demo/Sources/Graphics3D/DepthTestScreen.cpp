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
#include "DepthTestScreen.h"
#include "Graphics3D.h"
#include "Model.h"
#include "Shaders/Shader.h"

void DepthTestScreen::Start(){
	CCScene::Start();
	shader = new Shader("gfx3D/shaders/depth_test.vert", "gfx3D/shaders/depth_test.frag");
	//warrior = gfx3D->LoadModel("gfx3D/warrior/warrior.3DS", shader);
	warrior->SetRotateX(-90.f);
}

void DepthTestScreen::Stop(){
	CCScene::Stop();
	delete shader;
	delete warrior;
}

void DepthTestScreen::Update(float sec){/*
	gfx3D->DrawModel(shader, warrior);
	*/
	CCScene::Update(sec);
}