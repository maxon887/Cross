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
#include "Sprite.h"
#include "AudioScreen.h"
#include "Camera2D.h"
#include "Graphics3D/TriangleScreen.h"
#include "Graphics2D/PrimitivesScreen.h"
#include "Graphics2D/AnimationScreen.h"
#include "Graphics2D/TestNaPidoraScreen.h"
#include "Graphics2D/SpritesScreen.h"
#include "Graphics2D/TextScreen.h"
#include "Graphics2D/Camera2DScreen.h"

void MainScreen::Start(){
	ScrollScreen::Start();
	next_screen = nullptr;
	button_sprite = gfx2D->LoadImage("DefaultButton.png");
	button_sprite->SetScale(1.35f);
	//main menu
	main_menu = new Menu(true);
	current_menu = main_menu;
	Button* graphics2Dbtn = new Button("Graphics 2D");
	Button* graphics3Dbtn = new Button("Graphics 3D");
	Button* audioBtn = new Button("Audio");
	graphics2Dbtn->SetImages(button_sprite->Clone(), nullptr);
	graphics3Dbtn->SetImages(button_sprite->Clone(), nullptr);
	audioBtn->SetImages(button_sprite->Clone(), nullptr);
	graphics2Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics2DClick);
	graphics3Dbtn->Clicked += MakeDelegate(this, &MainScreen::OnGraphics3DClick);
	audioBtn->Clicked += MakeDelegate(this, &MainScreen::OnAudioClick);
	main_menu->AddButton(graphics2Dbtn);
	main_menu->AddButton(graphics3Dbtn);
	main_menu->AddButton(audioBtn);
	//graphics 2D menu
	graphics2D_menu = new Menu(false);
	Button* primitivesBtn = new Button("Primitives");
	Button* spritesBtn = new Button("Sprites");
	Button* textBtn = new Button("Text Drawing");
	Button* canvasBtn = new Button("2D Camera");
	Button* animationBtn = new Button("Animation");
	Button* testNaPidoraBtn = new Button("Test Na Pidora");
	primitivesBtn->SetImages(button_sprite->Clone());
	spritesBtn->SetImages(button_sprite->Clone());
	textBtn->SetImages(button_sprite->Clone());
	canvasBtn->SetImages(button_sprite->Clone());
	animationBtn->SetImages(button_sprite->Clone());
	testNaPidoraBtn->SetImages(button_sprite->Clone());
	primitivesBtn->Clicked += MakeDelegate(this, &MainScreen::OnPrimitivesClick);
	spritesBtn->Clicked += MakeDelegate(this, &MainScreen::OnSpritesClick);
	canvasBtn->Clicked += MakeDelegate(this, &MainScreen::On2DCameraClick);
	animationBtn->Clicked += MakeDelegate(this, &MainScreen::OnAnimationClick);
	testNaPidoraBtn->Clicked += MakeDelegate(this, &MainScreen::OnTestNaPidoraClick);
	textBtn->Clicked += MakeDelegate(this, &MainScreen::OnTextClick);
	//graphics 3D menu
	graphics3D_menu = new Menu(true);
	Button* triangleBtn = new Button("Triangle");
	triangleBtn->SetImages(button_sprite->Clone(), nullptr);
	triangleBtn->Clicked += MakeDelegate(this, &MainScreen::OnTriangleClick);
	graphics3D_menu->AddButton(triangleBtn);
	graphics2D_menu->AddButton(primitivesBtn);
	graphics2D_menu->AddButton(spritesBtn);
	graphics2D_menu->AddButton(textBtn);
	graphics2D_menu->AddButton(canvasBtn);
	graphics2D_menu->AddButton(animationBtn);
	graphics2D_menu->AddButton(testNaPidoraBtn);

	graphics2D_menu->Active(false);
	graphics3D_menu->Active(false);

	key_released_delegate = MakeDelegate(this, &MainScreen::KeyReleasedHandle);
	input->KeyPressed += key_released_delegate;
}

void MainScreen::Stop(){
	ScrollScreen::Stop();
	delete button_sprite;
	delete main_menu;
	delete graphics2D_menu;
	delete graphics3D_menu;
	input->KeyPressed -= key_released_delegate;
}

void MainScreen::Update(float sec){
	current_menu->Update(sec);

	if(next_screen){
		gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
		game->SetScreen(next_screen);
	}
}

void MainScreen::KeyReleasedHandle(Key key){
	if(current_menu == main_menu){
		launcher->PromtToExit();
	}
	current_menu->Active(false);
	main_menu->Active(true);
	current_menu = main_menu;
	float scrWidth = 1600.f;
	float scrHeight = scrWidth / launcher->GetAspectRatio();
	SetWidth(scrWidth);
	SetHeight(scrHeight);
	gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
}

void MainScreen::OnGraphics2DClick(){
	SetHeight(graphics2D_menu->GetHeight());
	Vector2D camPos(0.f, 0.f);
	camPos.y = GetHeight() - gfx2D->GetCamera()->GetViewHeight();
	gfx2D->GetCamera()->SetPosition(camPos);
	current_menu->Active(false);
	graphics2D_menu->Active(true);
	current_menu = graphics2D_menu;
}

void MainScreen::OnGraphics3DClick(){
	current_menu->Active(false);
	graphics3D_menu->Active(true);
	current_menu = graphics3D_menu;
}

void MainScreen::OnAnimationClick(){
	next_screen = new AnimationScreen();
}

void MainScreen::OnPrimitivesClick(){
	next_screen = new PrimitivesScreen();
}

void MainScreen::OnSpritesClick(){
	next_screen = new SpritesScreen();
}

void MainScreen::OnTestNaPidoraClick() {
	next_screen = new TestNaPidoraScreen();
}

void MainScreen::OnTextClick(){
	next_screen = new TextScreen();
}

void MainScreen::On2DCameraClick(){
	next_screen = new Camera2DScreen();
}

void MainScreen::OnTriangleClick(){
	next_screen = new TriangleScreen();
}

void MainScreen::OnAudioClick() {
	next_screen = new AudioScreen();
}