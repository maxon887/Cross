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
#include "NanosuitScreen.h"
#include "Graphics3D.h"
#include "Model.h"
#include "DirectionalLight.h"

void NanosuitScreen::Start(){
	CameraControlScreen::Start();
	orbit_distance = 60.f;

	nanosuit = gfx3D->LoadModel("gfx3D/nanosuit/nanosuit.fbx");
	nanosuit->SetPosition(Vector3D(0.f, -7.f, 0.f));
	//nanosuit = gfx3D->LoadModel("gfx3D/Cube.fbx");
}

void NanosuitScreen::Stop(){
	CameraControlScreen::Stop();
	delete nanosuit;
}

void NanosuitScreen::Update(float sec){
	gfx3D->DrawModelTexture(nanosuit);

	CameraControlScreen::Update(sec);
}