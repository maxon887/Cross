/*	Copyright © 2015 Vladas Zakrevskis

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
#include "TestNaPidoraScreen.h"
#include "Graphics2D.h"
#include "Game.h"
#include "AnimationScreen.h"
#include "MainScreen.h"
#include "Audio.h"
#include "Sprite.h"







void TestNaPidoraScreen::Update(float sec)
{
	gfx2D->Clear();
	
	gfx2D->DrawText(Vector2D(100, 100), "TI PIDOR))))))");

	gfx2D->DrawLine(Vector2D(200.f, 200.f), Vector2D(400.f, 150.f), Color::Green);
	gfx2D->DrawRect(Rect(50.f, 250.f, 50.f, 100.f), Color::Red);
	gfx2D->DrawRect(Rect(200.f, 50.f, 150.f, 70.f), Color(0.44f, 0.15f, 0.54f), true);
	gfx2D->DrawRect(Rect(150.f, 250.f, 50.f, 100.f), Color::Blue, false);
	gfx2D->DrawCircle(Vector2D(400.f, 400.f), 50.f, Color::Yellow);
	gfx2D->DrawCircle(Vector2D(350.f, 250.f), 25.f, Color::Red, true);
	gfx2D->DrawLine(Vector2D(50.f, 100.f), Vector2D(50.f, 200.f), Color::Red);
}

TestNaPidoraScreen::~TestNaPidoraScreen()
{
}

void TestNaPidoraScreen::OnKeyPressed(Key key)
{
	//if (key == ESCAPE)
		game->SetScreen(new MainScreen());
}
