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
#include "Game.h"
#include "Launcher.h"
#include "AnimationScreen.h"
#include "AudioScreen.h"
#include "TestScreen.h"
#include "Birds\BirdsScreen.h"
#include "Config.h"
#include "Sprite.h"
#include "TestNaPidoraScreen.h"

MainScreen::~MainScreen(){
	delete texter;
//	delete animation_btn;
	delete audio_btn;
//	delete primitives_btn;
//	delete misc_btn;
	delete x_img;
	delete internalScreenRedFont;
}

void MainScreen::Start(){
	going_screen = NO_SCREEN;

	Vector2D pos;

	texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 1.0f);
    x_img = gfx2D->LoadImage("Logo.png");

	Sprite* audioBtn = gfx2D->LoadImage("AudioButton.png");

	audio_btn = new Button(pos);
	audio_btn->SetImages(audioBtn, nullptr);
	audio_btn->Clicked += MakeDelegate(this, &MainScreen::OnAudioClick);

	test_label_button = new Button(pos, "TEST NA PIDORA");
	test_label_button->Clicked += MakeDelegate(this, &MainScreen::OnTestNaPidoraClick);




	int startLaunches = config->LoadInt("START_LAUNCHES", 0);
	startLaunches++;
	config->SaveInt("START_LAUNCHES", startLaunches);
	start_count = startLaunches;
	


	this->internalScreenRedFont = new Font("Engine/times.ttf", 80, Color::Red);
}

void MainScreen::Update(float sec){

	gfx2D->Clear();

	gfx2D->DrawText(Vector2D(0, 0), "bla bla bla");
	gfx2D->DrawText(Vector2D(0, 200), "text", internalScreenRedFont);


	audio_btn->SetLocation(Vector2D(game->GetWidth() / 2, 300));
	audio_btn->Update();

	test_label_button->SetLocation(Vector2D(game->GetWidth() / 2, 100));
	test_label_button->Update();

	switch (going_screen)
	{
	case NO_SCREEN:
		break;
	case ANIMATION:
		game->SetScreen(new AnimationScreen());
		break;
	case AUDIO:
		game->SetScreen(new AudioScreen());
		break;
	case TEST_NA_PIDORA:
		game->SetScreen(new TestNaPidoraScreen());
		break;
	case MISC:
		//game->SetScreen(new TestScreen());
		game->SetScreen(new BirdsScreen());
		break;
	default:
		break;
	}
}

void MainScreen::OnAnimationClick(){
	launcher->LogIt("OnClick");
	going_screen = ANIMATION;
}

void MainScreen::OnAudioClick(){
	going_screen = AUDIO;
}

void MainScreen::OnPrimitivesClick(){
	going_screen = PRIMITIVES;
}

void MainScreen::OnMiscClick(){
	going_screen = MISC;
}

void MainScreen::OnTestNaPidoraClick() {
	going_screen = TEST_NA_PIDORA;
}