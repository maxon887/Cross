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
#include "SecondScreen.h"

MainScreen::MainScreen(Game* game):Screen(game) { }

void MainScreen::Start(){
    x_img = graphics->LoadImage("X256.png");
	texter = new Texter(game);
	graphics->ScaleImage(x_img, game->GetScaleFactor() * 1.5f);
    Image* release = graphics->LoadImage("ButtonRelease.png");
	Image* pressed = graphics->LoadImage("ButtonPressed.png");
	PointX pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 4 * 3;
	btn = new Button(game, pos, release, pressed);
	btn->RegisterCallback(bind(&MainScreen::OnClick, this));
	Image* on = graphics->LoadImage("MusicOn.png");
	Image* off = graphics->LoadImage("MusicOff.png");
	pos.x = game->GetWidth() - on->GetWidth();
	pos.y = on->GetHeight();
	music_btn = new ToggleButton(game, pos, on, off);
	music_btn->RegisterCallback(bind(&MainScreen::MusicOnClick, this));

	int startLaunches = game->saver->LoadInt("START_LAUNCHES");
	startLaunches++;
	game->saver->SaveInt("START_LAUNCHES", startLaunches);
	start_count = startLaunches;

	bck_music = NULL;
	bck_music = launcher->CreateMusic("game_song.mp3", true);
	bck_music->Play();
}

void MainScreen::Update(float sec){
	graphics->Clear(0, 0.15f, 0.15f);

	PointX pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 3;
	graphics->Rotate(x_img, 15);
	graphics->DrawImage(pos, x_img);
	pos.x -= 200;
	pos.y += 300;
	toascii(start_count);
	string msg = "Screen starts ";
	msg += to_string(start_count);
	msg += " times";
	texter->DrawText(pos, msg.c_str());
	music_btn->Update();
	btn->Update();
}

void MainScreen::OnClick(){
	launcher->LogIt("OnClick");
    game->SetScreen(new SecondScreen(game));
}

void MainScreen::MusicOnClick(){
	if(music_btn->GetState())
		bck_music->Play();
	else
		bck_music->Pause();
}

MainScreen::~MainScreen(){
	graphics->ReleaseImage(x_img);
	delete btn;
	delete bck_music;
}
