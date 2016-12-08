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
#include "System/Debugger.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Launcher.h"
#include "Sprite.h"

Demo* demo = NULL;

Demo::Demo(Launcher* launcher) : Game() { }

void Demo::Start(){
	Game::Start();
	launcher->LogIt("Demo::Start()");
	demo = (Demo*)game;

	common_texture = gfx2D->LoadTexture("gfx2D/Common.png", Texture::Filter::LINEAR, false);
	gfx2D->LoadSprites(common_sprites, common_texture, "gfx2D/Common.xml");

	input->KeyPressed += MakeDelegate(this, &Demo::OnKeyPressed);

	camera = new Camera2D();
	camera->SetViewWidth(1600.f);

	back_btn = new Button();
    Sprite* arrowReleased = GetCommonSprite("ArrowUp.png");
    Sprite* arrowPressed = GetCommonSprite("ArrowDown.png");
    arrowReleased->SetRotate(180.f);
    arrowPressed->SetRotate(180.f);
    back_btn->SetImages(arrowReleased, arrowPressed);
	back_btn->Clicked += MakeDelegate(this, &Demo::OnBackClick);
}

void Demo::Stop(){
	launcher->LogIt("Demo::Stop()");
	delete back_btn;
	delete camera;
	for(std::pair<string, Sprite*> pair: common_sprites){
		delete pair.second;
	}
	delete common_texture;
	Game::Stop();
}

void Demo::Update(float sec){
	float height = gfx2D->GetCamera()->GetViewHeight();
	height += gfx2D->GetCamera()->GetPosition().y;
	Vector2D location(back_btn->GetWidth()/2.f, height - back_btn->GetHeight()/2.f);
    back_btn->SetLocation(location);
	back_btn->Update(sec);
}

Screen* Demo::GetStartScreen(){
	camera->SetPosition(Vector3D::Zero);
	gfx2D->SetCamera(camera);
	return new MainScreen();
}

Sprite* Demo::GetCommonSprite(string name){
	return common_sprites[name]->Clone();
}

void Demo::OnBackClick(){
    SetScreen(GetStartScreen());
}

void Demo::OnKeyPressed(Key key){
	if(key == Key::ESCAPE || key == Key::BACK){
		SetScreen(GetStartScreen());
	}
}
