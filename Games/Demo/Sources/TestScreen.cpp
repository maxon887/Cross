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
#include "TestScreen.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Game.h"
#include "MainScreen.h"
#include "Launcher.h"

TestScreen::~TestScreen(){
	gfx2D->ReleaseImage(grid);
	delete times_new_roman;
}

void TestScreen::Start(){
	grid = gfx2D->LoadImage("Grid.png");
	times_new_roman = new Font("Engine/times.ttf", 16, Color::Red);
}

void TestScreen::Update(float sec){
	gfx2D->Clear();
	float x = launcher->GetTargetWidth() / 2.f;
	float y = launcher->GetTargetHeight() / 2.f;
	gfx2D->DrawImage(Vector2D(x, y), grid);

	gfx2D->DrawText(Vector2D(-1.0f, -1.0f), "Blah", times_new_roman);

	if(input->IsPressed(Key::ESCAPE)){
		game->SetScreen(new MainScreen());
	}
}