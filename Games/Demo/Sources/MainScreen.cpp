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
	
#include "MainScreen.h"

MainScreen::MainScreen(Game* game):Screen(game) { }

void MainScreen::Start(){ 
	//x_img = graphics->LoadImage("h:/GameDev/Cross++/Games/Demo/Data/X256.png");
	//button_release = graphics->LoadImage("h:/GameDev/Cross++/Games/Demo/Data/ButtonRelease.png");
	//x_img = graphics->LoadImage("/data/data/com.cross/files/X256.png");
	//button_release = graphics->LoadImage("/data/data/com.cross/files/ButtonRelease.png");
    x_img = graphics->LoadImage("X256.png");
    button_release = graphics->LoadImage("ButtonRelease.png");
}

void MainScreen::Update(float sec){
	graphics->Clear(0, 0.25f, 0.25f);
	PointX pos(launcher->GetTargetWidth()/2, launcher->GetTargetHeight()/3);
	graphics->DrawTargetImage(pos, x_img);
	PointX butPoint(launcher->GetTargetWidth()/2, launcher->GetTargetHeight() / 4 * 3);
	graphics->DrawTargetImage(butPoint, button_release);
}
