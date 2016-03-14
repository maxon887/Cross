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
	testButton = new Button(Rect(Vector2D(100, 100), 500, 100), "fgdsgfdfgdfgdfghdfghdfghdgfhdgfh");
	testButton1 = new Button(Rect(Vector2D(100, 300), 500, 100), "bla");
	testButton2 = new Button(Rect(Vector2D(100, 500), 500, 100), "bgsfdgdfgsdfghghgfhsfla");

	
}

void TestNaPidoraScreen::Update(float sec)
{
	testButton->Update();
	testButton1->Update();
	testButton2->Update();

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)){
		game->SetScreen(new MainScreen());
	}
}

TestNaPidoraScreen::~TestNaPidoraScreen()
{
	if (testButton != nullptr) {
		delete testButton;
	}
	if (testButton != nullptr) {
		delete testButton1;
	}
	if (testButton != nullptr) {
		delete testButton2;
	}
}

void TestNaPidoraScreen::OnKeyPressed(Key key)
{
	//if (key == ESCAPE)
		game->SetScreen(new MainScreen());
}
