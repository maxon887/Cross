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

#include "MenuScreen.h"
#include "GameScreen.h"
#include "JimTheSnake.h"
#include "Misc.h"
#include <cmath>

MenuScreen::MenuScreen(JimTheSnake* game):Screen(game) {
	this->game = game;
}

void MenuScreen::Start(){
	launcher->LogIt("MenuScreen->Start()");
	sun_w = 5.f;
	sun_angle = 0;
	score = 0;
	//image loading
	background = graphics->LoadImage("Menu/Background.png");
	sun = graphics->LoadImage("Menu/Sun.jpg");
	bestscore = graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	jimthesnake = graphics->LoadImage("Menu/JimTheSnakeLabel.png");
	Image* playUp = graphics->LoadImage("Menu/PlayButtonUp.png");
	Image* playDown = graphics->LoadImage("Menu/PlayButtonDown.png");
	Image* soundUp = graphics->LoadImage("Menu/SoundUp.png", game->GetScaleFactor() * 1.2f);
	Image* soundDown = graphics->LoadImage("Menu/SoundDown.png", game->GetScaleFactor() * 1.2f);
	Image* musicUp = graphics->LoadImage("Menu/MusicUp.png", game->GetScaleFactor() * 1.2f);
	Image* musicDown = graphics->LoadImage("Menu/MusicDown.png", game->GetScaleFactor() * 1.2f);
	//positioning
	float scaleFactor = 0;
	float imageAspect = background->GetWidth() / background->GetHeight();
	float deviceAspect = (float)launcher->GetTargetWidth() / launcher->GetTargetHeight();
	if(imageAspect < deviceAspect)
		scaleFactor = launcher->GetTargetWidth() / background->GetWidth();
	else
		scaleFactor = launcher->GetTargetHeight() / background->GetHeight();
	sun_pos.x = 400.f * scaleFactor / game->GetScaleFactor();
	sun_pos.y = 580.f * scaleFactor / game->GetScaleFactor();
	snaky_pos.x = 200.f * scaleFactor / game->GetScaleFactor();
	snaky_pos.y = 120.f * scaleFactor / game->GetScaleFactor();
	background_pos.x = game->GetWidth() / 2.f;
	background_pos.y = game->GetHeight() / 2.f;
	Point playPos;
	playPos.x = 270.f * scaleFactor / game->GetScaleFactor();
	playPos.y = 490.f * scaleFactor / game->GetScaleFactor();
	Point soundPos;
	soundPos.x = soundUp->GetWidth() * 0.8f;
	soundPos.y = game->GetHeight() - soundUp->GetHeight() * 0.8f;
	Point musicPos;
	musicPos.x = musicUp->GetWidth() * 2.2f;
	musicPos.y = game->GetHeight() - musicUp->GetHeight() * 0.8f;
	//scaling
	graphics->ScaleImage(background, scaleFactor);
	graphics->ScaleImage(sun, scaleFactor);
	graphics->ScaleImage(jimthesnake, scaleFactor * 0.9f);
	//buttons creation
	play_btn = new Button(game, playPos, playUp, playDown);
	play_btn->RegisterCallback(bind(&MenuScreen::OnPlayClick, this));
	sound_btn = new ToggleButton(game, soundPos, soundUp, soundDown);
	sound_btn->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	music_btn = new ToggleButton(game, musicPos, musicUp, musicDown);
	music_btn->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));
	//misc
	score_texter = new Texter(game, "NumbersYellow.png", 65.f, 76.f, 10, 1, 48);
	score = game->BestScore();
	sound_btn->SetState(game->IsSoundEnabled());
	menu_music = 0;
	menu_music->Play();
	menu_music = new Audio("Menu/MenuMusic.mp3", true, true);
	music_btn->SetState(game->IsMusicEnabled());
	if(game->IsMusicEnabled()){
		menu_music->Play();
	}
	CreateDeadAreas();
}

void MenuScreen::Update(float sec){
 	graphics->Clear(0.0f, 0.25f, 0.25f);
	UpdateSun(sec);
	graphics->DrawImage(background_pos, background);

	graphics->DrawImage(snaky_pos, jimthesnake);
	float offset = bestscore->GetHeight() / 2;
	graphics->DrawImage(Point(game->GetWidth() / 2 + 100.f, game->GetHeight() - offset - 20.f), bestscore);
	score_texter->DrawText(Point(game->GetWidth() / 2 + 170.f + 100.f, game->GetHeight() - offset - 55.f), to_string(score));
	/*
	for(Rect area : dead_areas){
		graphics->DrawRect(area, Color::Red);
	}*/

	music_btn->Update();
	sound_btn->Update();
	play_btn->Update();
}

void MenuScreen::CreateDeadAreas(){
	Rect r = play_btn->GetRect();
	dead_areas.push_back(r);
	r.x = 0;
	r.y = play_btn->GetCenter().y;
	r.width = game->GetWidth();
	r.height = game->GetHeight() - r.y;
	dead_areas.push_back(r);
	r.x = 0;
	r.y = 570;
	r.width = 250;
	r.height = game->GetHeight() - r.y;
	dead_areas.push_back(r);
}

void MenuScreen::UpdateSun(float sec){
	static float startAngle = 0;
	static float deltaAngle = 0;
	static float deltaTime = 0;
	static float lerpVal = 0;
	bool touchInDeadArea = false;

	if(input->HaveInput() && startAngle == 0){
		for(unsigned int i = 0; i < dead_areas.size(); i++){
			if(PointInRect(input->GetInput(), dead_areas[i])){
				touchInDeadArea = true;
			}
		}
	}

	if(input->HaveInput() && startAngle == 0 && !touchInDeadArea){
		float sun_tangens = (sun_pos.x - input->GetInput().x) / (sun_pos.y - input->GetInput().y);
		startAngle = atan(sun_tangens);
		startAngle = (float)(startAngle * 180.f / PI);
	}

	if(!input->HaveInput() && startAngle != 0 && !touchInDeadArea){
		sun_angle = sun_angle + deltaAngle;
		if(deltaAngle < -90){
			deltaAngle += 180.f;
		}
		if(deltaAngle > 90){
			deltaAngle -= 180.f;
		}
		sun_w = deltaAngle / deltaTime;
		startAngle = 0;
		deltaTime = 0;
		lerpVal = 0;
	}

	if(input->HaveInput() && !touchInDeadArea){
		float sun_tangens = (sun_pos.x - input->GetInput().x) / (sun_pos.y - input->GetInput().y);
		deltaAngle = atan(sun_tangens);
		deltaAngle = (float)(deltaAngle * 180.f / PI);
		deltaAngle = deltaAngle - startAngle;
		graphics->Rotate(sun, sun_angle + deltaAngle);
		graphics->DrawImage(sun_pos, sun);
		deltaTime += sec;
	}else{
		graphics->Rotate(sun, sun_angle);
		graphics->DrawImage(sun_pos, sun);
		sun_angle += sec * sun_w;
		if(lerpVal < 1)
			lerpVal += 0.05f * sec;
		else 
			lerpVal = 1;
		sun_w = Lerp(sun_w, 5.f, lerpVal);
	}
}

void MenuScreen::OnPlayClick(){
	game->SetScreen(new GameScreen(game));
}

void MenuScreen::OnMusicClick(){
	game->SetMusicEnabled(music_btn->GetState());
	if(music_btn->GetState()){
		menu_music->Play();
	}else{
		menu_music->Stop();
	}
}

void MenuScreen::OnSoundClick(){
	game->SetSoundEnabled(sound_btn->GetState());
}

MenuScreen::~MenuScreen(){
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(sun);
	graphics->ReleaseImage(bestscore);
	graphics->ReleaseImage(jimthesnake);
	delete play_btn;
	delete music_btn;
	delete sound_btn;
	delete menu_music;
	delete score_texter;
}
