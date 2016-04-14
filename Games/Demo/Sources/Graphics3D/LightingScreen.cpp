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
#include "LightingScreen.h"
#include "Game.h"
#include "Graphics3D.h"
#include "Model.h"

void LightingScreen::Start(){
	CameraControlScreen::Start();
	gfx3D->GetCamera()->SetPosition(Vector3D(0.f, 0.f, -28.f));
	orbit_distance = 28.f;
	cube = gfx3D->LoadModel(Shader::LIGHT, "gfx3D/Cube.obj");
	cube->SetColor(Color(1.f, 0.33f, 0.f));
	light = new Light(Color::White);
	light->SetPosition(Vector3D(10.f, 8.f, -5.f));
	gfx3D->AddLightSource(light);
}

void LightingScreen::Stop(){
	CameraControlScreen::Stop();
	delete cube;
	delete light;
}

void LightingScreen::Update(float sec){
	cube->Draw();
	light->Draw();

	CameraControlScreen::Update(sec);
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}