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
#include "CubeScreen.h"
#include "Game.h"
#include "Graphics3D.h"
#include "Model.h"

void CubeScreen::Start(){
	CameraControlScreen::Start();
	cube = gfx3D->LoadModel("gfx3D/CubeMaxTriangles.obj");
}

void CubeScreen::Stop(){
	CameraControlScreen::Stop();
}

void CubeScreen::Update(float sec){
	cube->Draw();

	CameraControlScreen::Update(sec);
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}