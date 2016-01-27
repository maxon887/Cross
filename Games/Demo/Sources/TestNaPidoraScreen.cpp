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
#include "Launcher.h"

TestNaPidoraScreen::TestNaPidoraScreen()
{
	//fooButton = new Button(100, 100, "I'm button!!!!!");
	//fooButton2 = new Button("knoa");
	fooButton3 = new Button("Ano pk!aaa");

	//fooButton2->SetLocation(Vector2D(250, 370));
	//fooButton3->SetRect(Rect(0.f, 0.f, 50.f, 50.f));
	//Sprite* upSprite = gfx2D->LoadImage("DefaultButton.png");
	//fooButton3->SetImages(upSprite, nullptr);
	fooButton3->SetLocation(Vector2D(250, 250));
}

void TestNaPidoraScreen::Update(float sec)
{
	gfx2D->Clear();
	//launcher->Sleep(1000);
	//gfx2D->DrawText(Vector2D(100, 100), "TI PIDOR))))))");

	//fooButton->Update();
	//fooButton2->Update();
	fooButton3->Update();

	if(input->IsPressed(Key::ESCAPE)){
		game->SetScreen(new MainScreen());
	}
}

TestNaPidoraScreen::~TestNaPidoraScreen()
{
	delete fooButton3;
}

void TestNaPidoraScreen::OnKeyPressed(Key key)
{
	//if (key == ESCAPE)
		game->SetScreen(new MainScreen());
}
