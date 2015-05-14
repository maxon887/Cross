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
#define PI (3.141592653589793)

MenuScreen::MenuScreen(Game* game):Screen(game) { 
	background = NULL;
	sun = NULL;
	sun_w = 5.f;
	sun_angle = 0;
	score = 0;
}

void MenuScreen::Start(){
	background = graphics->LoadImage("Menu/Background.png");
	float scaleFactor = 0;
	float imageAspect = background->GetWidth() / background->GetHeight();
	float deviceAspect = (float)launcher->GetTargetWidth() / launcher->GetTargetHeight();
	if(imageAspect < deviceAspect)
		scaleFactor = launcher->GetTargetWidth() / background->GetWidth();
	else
		scaleFactor = launcher->GetTargetHeight() / background->GetHeight();
	graphics->ScaleImage(background, scaleFactor);
	background_pos.x = game->GetWidth() / 2.f;
	background_pos.y = game->GetHeight() / 2.f;

	sun = graphics->LoadImage("Menu/Sun.jpg", scaleFactor);
	sun_pos.x = 400.f * scaleFactor / game->GetScaleFactor();
	sun_pos.y = 580.f * scaleFactor / game->GetScaleFactor();
	//Buttons
	Image* playUp = graphics->LoadImage("Menu/PlayButtonUp.png");
	Image* playDown = graphics->LoadImage("Menu/PlayButtonDown.png");
	PointX playPos;
	playPos.x = 270.f * scaleFactor / game->GetScaleFactor();
	playPos.y = 490.f * scaleFactor / game->GetScaleFactor();
	play_btn = new Button(game, playPos, playUp, playDown);
	play_btn->RegisterCallback(bind(&MenuScreen::OnPlayClick, this));
	Image* soundUp = graphics->LoadImage("Menu/SoundUp.png", game->GetScaleFactor() * 1.2f);
	Image* soundDown = graphics->LoadImage("Menu/SoundDown.png", game->GetScaleFactor() * 1.2f);
	PointX soundPos;
	soundPos.x = soundUp->GetWidth() * 0.8f;
	soundPos.y = game->GetHeight() - soundUp->GetHeight() * 0.8f;
	sound_btn = new ToggleButton(game, soundPos, soundUp, soundDown);
	sound_btn->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	Image* musicUp = graphics->LoadImage("Menu/MusicUp.png", game->GetScaleFactor() * 1.2f);
	Image* musicDown = graphics->LoadImage("Menu/MusicDown.png", game->GetScaleFactor() * 1.2f);
	PointX musicPos;
	musicPos.x = musicUp->GetWidth() * 2.2f;
	musicPos.y = game->GetHeight() - musicUp->GetHeight() * 0.8f;
	music_btn = new ToggleButton(game, musicPos, musicUp, musicDown);
	music_btn->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));

	score_texter = ((SnakyGame*)game)->score_texter;
	score_texter->SetScaleFactor(game->GetScaleFactor());
	score = saver->LoadInt(KEY_SCORE);
	bestscore = graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	bestscore_pos.x = 300.f;
	bestscore_pos.y = 520.f;
	snaky = graphics->LoadImage("Menu/SnakyLabel.png", scaleFactor);
	snaky_pos.x = 200.f * scaleFactor / game->GetScaleFactor();
	snaky_pos.y = 100.f * scaleFactor / game->GetScaleFactor();

	CreateDeadAreas();
}

void MenuScreen::Update(float sec){
	graphics->Clear(0.0f, 0.25f, 0.25f);
	UpdateSun(sec);
	graphics->DrawImage(background_pos, background);

	graphics->DrawImage(snaky_pos, snaky);
	graphics->DrawImage(bestscore_pos, bestscore);
	score_texter->DrawText(510, 490, to_string(score));

	//ShowDeadAreas();

	music_btn->Update();
	sound_btn->Update();
	play_btn->Update();
}

void MenuScreen::ShowDeadAreas(){
	for(unsigned int i = 0; i < dead_areas.size(); i++){
		graphics->DrawRect(dead_areas[i], ColorX::Red);
	}
}

void MenuScreen::CreateDeadAreas(){
	RectX r = play_btn->GetRect();
	dead_areas.push_back(r);
	r.x = 0;
	r.y = play_btn->GetCenter().y;
	r.width = game->GetWidth();
	r.height = game->GetHeight() - r.y;
	dead_areas.push_back(r);
	r.x = 0;
	r.y = bestscore_pos.y + 50;
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

}

void MenuScreen::OnSoundClick(){

}