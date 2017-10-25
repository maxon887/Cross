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
#include "Demo.h"
#include "MainScreen.h"
#include "Camera2D.h"
#include "System.h"
#include "Texture.h"
#include "Utils/Sprite.h"
#include "Utils/PrimitiveDrawer.h"

Demo* demo = NULL;

Demo::Demo() : 
	Game()
{ }

void Demo::Start(){
	Game::Start();
	system->LogIt("Demo::Start()");
	demo = (Demo*)game;

	common_texture = new Texture();
	common_texture->Load("gfx2D/Common.png", Texture::TilingMode::CLAMP_TO_EDGE, Texture::Filter::LINEAR, false);
	PrimitiveDrawer::LoadSprites(common_sprites, common_texture, "gfx2D/Common.xml");

	camera = new Camera2D();
	camera->SetViewWidth(1600.f);

	input->KeyPressed.Connect(this, &Demo::OnKeyPressed);
}

void Demo::Stop(){
	system->LogIt("Demo::Stop()");
	delete camera;
	for(std::pair<string, Sprite*> pair: common_sprites){
		delete pair.second;
	}
	delete common_texture;
	Game::Stop();
}

void Demo::Update(float sec){
	float height = GetCurrentScreen()->GetCamera()->GetViewHeight();
	height += GetCurrentScreen()->GetCamera()->GetPosition().y;
}

Screen* Demo::GetStartScreen(){
	return new MainScreen();
}

Sprite* Demo::GetCommonSprite(string name){
	return common_sprites[name]->Clone();
}

void Demo::OnBackClick(){
    SetScreen(GetStartScreen());
}

void Demo::OnKeyPressed(Key key){
	if(key == Key::ESCAPE){
		SetScreen(GetStartScreen());
	}
}
