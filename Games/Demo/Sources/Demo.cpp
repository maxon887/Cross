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
#include "Utils/Debugger.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Launcher.h"
#include "Sprite.h"

Demo* demo = nullptr;

Demo::Demo(Launcher* launcher) : Game() { }

void Demo::Start(){
	Game::Start();
	launcher->LogIt("Demo::Start()");
	demo = (Demo*)game;

	Debugger::Instance()->EnableDebug(Debugger::Parameter::FPS);
#ifndef ANDROID
	Debugger::Instance()->EnableDebug(Debugger::Parameter::UPDATE_TIME);
	Debugger::Instance()->EnableDebug(Debugger::Parameter::RUN_TIME);
	Debugger::Instance()->EnableDebug(Debugger::Parameter::RENDER_TIME);
	Debugger::Instance()->EnableDebug(Debugger::Parameter::INPUT);
#endif 

	camera = new Camera2D();
	camera->SetViewWidth(1600.f);

	common_texture = gfx2D->LoadTexture("gfx2D/Common.png", Texture::Filter::LINEAR, false);
	gfx2D->LoadSprites(common_sprites, common_texture, "gfx2D/Common.xml");
}

void Demo::Stop(){
	launcher->LogIt("Demo::Stop()");
	delete camera;
	for(std::pair<string, Sprite*> pair: common_sprites){
		delete pair.second;
	}
	delete common_texture;
	Game::Stop();
}

Screen* Demo::GetStartScreen(){
	gfx2D->SetCamera(camera);
	return new MainScreen();
}

Sprite* Demo::GetCommonSprite(string name){
	return common_sprites[name]->Clone();
}