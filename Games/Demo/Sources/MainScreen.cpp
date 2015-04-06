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

#include "SecondScreen.h"


MainScreen::MainScreen(Game* game):Screen(game) { }

void MainScreen::Start(){
    x_img = graphics->LoadImage("X256.png");
	graphics->ScaleImage(x_img, game->GetScaleFactor() * 1.5f);
    Image* release = graphics->LoadImage("ButtonRelease.png");
	Image* pressed = graphics->LoadImage("ButtonPressed.png");
	PointX pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 4 * 3;
	btn = new Button(game, pos, release, pressed);
	btn->RegisterCallback(bind(&MainScreen::OnClick, this));
}

void MainScreen::Update(float sec){
	graphics->Clear(0, 0.25f, 0.25f);

	PointX pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 3;
	graphics->Rotate(x_img, 15);
	graphics->DrawImage(pos, x_img);
	btn->Update();
	//pos.y = game->GetHeight() / 4 * 3;
	//graphics->DrawImage(pos, button_release);
}

void MainScreen::OnClick(){
	launcher->LogIt("OnClick");
    game->SetScreen(new SecondScreen(game));
}