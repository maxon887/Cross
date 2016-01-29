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
#include "SpritesScreen.h"

MainScreen::~MainScreen(){
	delete def_button;
	delete main_menu;
	delete graphics2D_menu;
}

void MainScreen::Start(){
	going_screen = NO_SCREEN;
	def_button = gfx2D->LoadImage("DefaultButton.png");
	gfx2D->SetClearColor(Color::Black);
	//main menu
	main_menu = new Menu();
	current_menu = main_menu;
	Button* graphics2Dbtn = new Button("Graphics 2D");
	Button* graphics3Dbtn = new Button("Graphics 3D");
	Button* audioBtn = new Button("Audio");
	graphics2Dbtn->SetImages(new Sprite(*def_button), nullptr);
	graphics3Dbtn->SetImages(new Sprite(*def_button), nullptr);
	audioBtn->SetImages(new Sprite(*def_button), nullptr);
	graphics2Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics2DClick);
	main_menu->AddButton(graphics2Dbtn);
	main_menu->AddButton(graphics3Dbtn);
	main_menu->AddButton(audioBtn);
	//graphics 2D menu
	graphics2D_menu = new Menu();
	Button* primitivesBtn = new Button("Primitives");
	Button* spritesBtn = new Button("Sprites");
	Button* textBtn = new Button("Text Drawing");
	Button* animationBtn = new Button("Animation");
	Button* testNaPidoraBtn = new Button("Test Na Pidora");
	primitivesBtn->SetImages(new Sprite(*def_button), nullptr);
	spritesBtn->SetImages(new Sprite(*def_button), nullptr);
	textBtn->SetImages(new Sprite(*def_button), nullptr);
	animationBtn->SetImages(new Sprite(*def_button), nullptr);
	testNaPidoraBtn->SetImages(new Sprite(*def_button), nullptr);
	primitivesBtn->Clicked += MakeDelegate(this, &MainScreen::OnPrimitivesClick);
	spritesBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpritesClick);
	testNaPidoraBtn->Clicked += MakeDelegate(this, &MainScreen::OnTestNaPidoraClick);

	graphics2D_menu->AddButton(primitivesBtn);
	graphics2D_menu->AddButton(spritesBtn);
	graphics2D_menu->AddButton(textBtn);
	graphics2D_menu->AddButton(animationBtn);
	graphics2D_menu->AddButton(testNaPidoraBtn);
	graphics2D_menu->Active(false);
}

void MainScreen::Update(float sec){
	gfx2D->Clear();
	current_menu->Update(sec);
	/*
	static float time = 1;
	time += sec;
	if((menu->Size() + 1) < time){
		Sprite* btnSprite = gfx2D->LoadImage("DefaultButton.png");
		Button* btn = new Button("Graphics 2D");
		btn->SetImages(btnSprite, nullptr);
		menu->AddButton(btn);
		if(menu->Size() > 10){
			menu->Clear();
			time = 1;
		}
	}*/

	if(input->IsPressed(Key::ESCAPE)){
		current_menu->Active(false);
		main_menu->Active(true);
		current_menu = main_menu;
	}

	switch (going_screen){
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
	case SPRITES:
		game->SetScreen(new SpritesScreen());
		break;
	case PRIMITIVES:
		game->SetScreen(new PrimitivesScreen());
		break;
	default:
		break;
	}
}

void MainScreen::OnGraphics2DClick(){
	current_menu->Active(false);
	graphics2D_menu->Active(true);
	current_menu = graphics2D_menu;
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

void MainScreen::OnSpritesClick(){
	going_screen = SPRITES;
}

void MainScreen::OnTestNaPidoraClick() {
	going_screen = TEST_NA_PIDORA;
}