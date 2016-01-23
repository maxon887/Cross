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

MainScreen::~MainScreen(){
	delete texter;
//	delete animation_btn;
	delete audio_btn;
//	delete primitives_btn;
//	delete misc_btn;
	delete x_img;
}

void MainScreen::Start(){
	going_screen = NO_SCREEN;
	//input->ActionDown.Clear();
	//input->ActionMove.Clear();
	//input->ActionUp.Clear();
	Vector2D pos;
	//input->KeyPressed.Clear();
	//input->KeyReleased.Clear();
	texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 1.0f);
    x_img = gfx2D->LoadImage("Logo.png");
	//graphics->ScaleImage(x_img, game->GetScaleFactor());
	//Sprite* animationBtn = gfx2D->LoadImage("AnimationButton.png");
	Sprite* audioBtn = gfx2D->LoadImage("AudioButton.png");
	//Sprite* primitivesBtn = gfx2D->LoadImage("PrimitivesButton.png");
	//Sprite* miscBtn = gfx2D->LoadImage("MiscButton.png");
//	animation_btn = new Button(game, pos, animationBtn, NULL);
	audio_btn = new Button(pos, audioBtn, NULL);
//	primitives_btn = new Button(game, pos, primitivesBtn, NULL);
//	misc_btn = new Button(game, pos, miscBtn, NULL);
//	animation_btn->Clicked += MakeDelegate(this, &MainScreen::OnAnimationClick);
	audio_btn->Clicked += MakeDelegate(this, &MainScreen::OnAudioClick);
//	primitives_btn->Clicked += MakeDelegate(this, &MainScreen::OnPrimitivesClick);
	//misc_btn->Clicked += MakeDelegate(this, &MainScreen::OnMiscClick);

	int startLaunches = config->LoadInt("START_LAUNCHES", 0);
	startLaunches++;
	config->SaveInt("START_LAUNCHES", startLaunches);
	start_count = startLaunches;
	
	//delete texterAdvanced;

	this->internalScreenRedFont = new Font("Engine/times.ttf", 80, Color::Red);
}

void MainScreen::Update(float sec){

	gfx2D->Clear();

	gfx2D->DrawText(Vector2D(0, 0), "bla bla bla");
	gfx2D->DrawText(Vector2D(0, 200), "text", internalScreenRedFont);

	//gfx2D->DrawLine(Vector2D(0,0), Vector2D(24,24), Color::Blue);
	Vector2D pos;
	pos.x = game->GetWidth() / 2;
	pos.y = 150;

	//animation_btn->SetLocation(pos);
	//animation_btn->Update();
	pos.y += 150;
	audio_btn->SetLocation(pos);
	audio_btn->Update();
	pos.y += 150;
	//primitives_btn->SetLocation(pos);
	//primitives_btn->Update();
	pos.y += 150;
//	misc_btn->SetLocation(pos);
	//misc_btn->Update();

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
	case PRIMITIVES:
		break;
	case MISC:
		//game->SetScreen(new TestScreen());
		game->SetScreen(new BirdsScreen());
		break;
	default:
		break;
	}
	//gfx2D->DrawText(Vector2D(-1.0f, -1.0f), "Blah");
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