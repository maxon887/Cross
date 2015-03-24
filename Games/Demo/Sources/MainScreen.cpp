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
	graphics->ScaleImage(x_img, game->scale_factor * 1.5f);
    button_release = graphics->LoadImage("ButtonRelease.png");
	background = graphics->LoadImage("Background.jpg");
	texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 2.0f);
}

void MainScreen::Update(float sec){
	graphics->Clear(0, 0.25f, 0.25f);
	PointX pos;
	pos.x = game->width / 2;
	pos.y = game->height / 3;
	graphics->Rotate(x_img, 15);
	graphics->DrawImage(pos, x_img);
	pos.y = game->height / 4 * 3;
	graphics->DrawImage(pos, button_release);

	texter->DrawText(30, 100, "Hello");
}
