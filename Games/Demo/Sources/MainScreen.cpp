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
#include "Graphics2D.h"
#include "Launcher.h"
#include "AnimationScreen.h"
#include "AudioScreen.h"
#include "PrimitivesScreen.h"
#include "Birds\BirdsScreen.h"
#include "Config.h"
#include "Sprite.h"
#include "TestNaPidoraScreen.h"

MainScreen::~MainScreen(){
	//delete audio_btn;
	//delete test_label_button;
}

void MainScreen::Start(){
	going_screen = NO_SCREEN;
	menu = new Menu();
}

void MainScreen::Update(float sec){
	gfx2D->Clear();
	static float time = 1;
	time += sec;
	menu->Update(sec);

	if((menu->Size() + 1) < time){
		Sprite* audioBtn = gfx2D->LoadImage("AudioButton.png");
		Button* btn = new Button(100, 100, "Top knopka");
		menu->AddButton(btn);
		if(menu->Size() > 10){
			menu->Clear();
			time = 1;
		}
	}

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