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

MainScreen::MainScreen(Game* game):Screen(game) {
	bck_music = NULL;
	jaguar = NULL;
	truck = NULL;
}

void MainScreen::Start(){
	Point pos;
	texter = new Texter(game, "Font.png", 11.0f, 20.0f, 23, 6, 32, 1.0f);
    x_img = graphics->LoadImage("X256.png");
	graphics->ScaleImage(x_img, game->GetScaleFactor() * 1.5f);
    Image* release = graphics->LoadImage("StartButtonUp.png");
	Image* pressed = graphics->LoadImage("StartButtonDown.png");
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 4 * 2;
	btn = new Button(game, pos, release, pressed);
	btn->RegisterCallback(bind(&MainScreen::OnClick, this));
	Image* on = graphics->LoadImage("MusicOn.png");
	Image* off = graphics->LoadImage("MusicOff.png");
	pos.x = game->GetWidth() - on->GetWidth();
	pos.y = on->GetHeight();
	music_btn = new ToggleButton(game, pos, on, off);
	music_btn->RegisterCallback(bind(&MainScreen::MusicOnClick, this));
	//bck_music = new Audio("Eskimo.mp3", true, true);
	bool musicState = saver->LoadBool("MUSIC_STATE");
	music_btn->SetState(musicState);
	if(musicState){
		bck_music->Play();
		song_started = true;
	}else{
		song_started = false;
	}

	Image* yellow_img = graphics->LoadImage("SoundButtonYellow.png");
	Image* blue_img = graphics->LoadImage("SoundButtonBlue.png");
	Image* gray_img = graphics->LoadImage("SoundButtonGray.png");
	yellow_sound_btn = new Button(game, yellow_img, gray_img);
	pos.x = yellow_sound_btn->GetWidth() / 2;
	pos.y = game->GetHeight() - yellow_sound_btn->GetHeight() / 2;
	yellow_sound_btn->SetLocation(pos);
	yellow_sound_btn->RegisterCallback(bind(&MainScreen::OnYellowClick, this));
	blue_sound_btn = new Button(game, blue_img, gray_img);
	pos.x = game->GetWidth() - blue_sound_btn->GetWidth() / 2;
	pos.y = game->GetHeight() - blue_sound_btn->GetHeight() / 2;
	blue_sound_btn->SetLocation(pos);
	blue_sound_btn->RegisterCallback(bind(&MainScreen::OnBlueClick, this));

	//jaguar = new Audio("Jaguar.wav", false, false);
	//truck = new Audio("Truck.wav", true, false);

	int startLaunches = game->saver->LoadInt("START_LAUNCHES");
	startLaunches++;
	game->saver->SaveInt("START_LAUNCHES", startLaunches);
	start_count = startLaunches;
}

void MainScreen::Update(float sec){
	graphics->Clear(0, 0.15f, 0.15f);

	Point pos;
	pos.x = game->GetWidth() / 2;
	pos.y = game->GetHeight() / 3 - 120;
	graphics->Rotate(x_img, 15);
	graphics->DrawImage(pos, x_img);
	pos.x -= 200;
	pos.y = game->GetHeight() / 5 * 3 + 50;
	string msg = "Screen starts " + to_string(start_count) + " times";
	texter->DrawText(pos, msg.c_str());
	music_btn->Update();
	yellow_sound_btn->Update();
	blue_sound_btn->Update();
	btn->Update();
}

void MainScreen::OnClick(){
	launcher->LogIt("OnClick");
    game->SetScreen(new SecondScreen(game));
}

void MainScreen::OnYellowClick(){
	jaguar->Play();
}

void MainScreen::OnBlueClick(){
	if(truck->IsPlaying()){
		truck->Stop();
	}else{
		truck->Play();
	}
}

void MainScreen::MusicOnClick(){
	if(music_btn->GetState()){
		if(song_started)
			bck_music->Resume();
		else
			bck_music->Play();
	}
	else
		bck_music->Pause();
	saver->SaveBool("MUSIC_STATE", music_btn->GetState());
}

MainScreen::~MainScreen(){
	graphics->ReleaseImage(x_img);
	delete btn;
	delete bck_music;
}
