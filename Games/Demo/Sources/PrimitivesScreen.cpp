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
#include "PrimitivesScreen.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Game.h"
#include "MainScreen.h"
#include "Launcher.h"
#include "Misc.h"

PrimitivesScreen::~PrimitivesScreen(){
}

void PrimitivesScreen::Start(){
}

void PrimitivesScreen::Update(float sec){
	gfx2D->Clear();
	float x = launcher->GetTargetWidth() / 2.f;
	float y = launcher->GetTargetHeight() / 2.f;

	for(int i = 0; i < 10; i++){
		gfx2D->DrawPoint(Vector2D(Random(25, 75), Random(25, 75)), Color::Red);
		gfx2D->DrawPoint(Vector2D(Random(25, 75), Random(25, 75)), Color::Green);
	}
	gfx2D->DrawLine(Vector2D(200, 200), Vector2D(400, 150), Color::Green);
	gfx2D->DrawRect(Rect(50, 250, 50, 100), Color::Red);
	gfx2D->DrawRect(Rect(200, 50, 150, 70), Color(0.44f, 0.15f, 0.54f), true);
	gfx2D->DrawRect(Rect(150, 250, 50, 100), Color::Blue, false);
	gfx2D->DrawCircle(Vector2D(400, 400), 50, Color::Yellow);
	gfx2D->DrawCircle(Vector2D(350, 250), 25, Color::Red, true);
	gfx2D->DrawLine(Vector2D(50, 100), Vector2D(50, 200), Color::Red);

	if(input->IsPressed(Key::ESCAPE)){
		game->SetScreen(new MainScreen());
	}
}