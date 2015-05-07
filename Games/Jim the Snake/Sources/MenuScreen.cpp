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
float lerp(float v0, float v1, float t) {
  return (1-t)*v0 + t*v1;
}

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
	//background_pos.x = launcher->GetTargetWidth() / 2.f / game->GetScaleFactor();
	//background_pos.y = launcher->GetTargetHeight() / 2.f / game->GetScaleFactor();
	background_pos.x = game->GetWidth() / 2;
	background_pos.y = game->GetHeight() / 2;

	sun = graphics->LoadImage("Menu/Sun.jpg", scaleFactor);
	sun_pos.x = 400 * scaleFactor / game->GetScaleFactor();
	sun_pos.y = 580 * scaleFactor / game->GetScaleFactor();
	//Buttons
	Image* playUp = graphics->LoadImage("Menu/PlayButtonUp.png");
	Image* playDown = graphics->LoadImage("Menu/PlayButtonDown.png");
	PointX playPos;
	playPos.x = 270 * scaleFactor / game->GetScaleFactor();
	playPos.y = 490 * scaleFactor / game->GetScaleFactor();
	play_btn = new Button(game, playPos, playUp, playDown);
	play_btn->RegisterCallback(bind(&MenuScreen::OnPlayClick, this));
	Image* soundUp = graphics->LoadImage("Menu/SoundUp.png", game->GetScaleFactor() * 1.2f);
	Image* soundDown = graphics->LoadImage("Menu/SoundDown.png", game->GetScaleFactor() * 1.2f);
	PointX soundPos;
	soundPos.x = soundUp->GetWidth() * 0.8;
	soundPos.y = game->GetHeight() - soundUp->GetHeight() * 0.8;
	sound_btn = new ToggleButton(game, soundPos, soundUp, soundDown);
	sound_btn->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	Image* musicUp = graphics->LoadImage("Menu/MusicUp.png", game->GetScaleFactor() * 1.2f);
	Image* musicDown = graphics->LoadImage("Menu/MusicDown.png", game->GetScaleFactor() * 1.2f);
	PointX musicPos;
	musicPos.x = musicUp->GetWidth() * 2.2;
	musicPos.y = game->GetHeight() - musicUp->GetHeight() * 0.8;
	music_btn = new ToggleButton(game, musicPos, musicUp, musicDown);
	music_btn->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));

	score_texter = ((SnakyGame*)game)->score_texter;
	score_texter->SetScaleFactor(game->GetScaleFactor());
	score = saver->LoadInt(KEY_SCORE);
	bestscore = graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	bestscore_pos.x = 300;
	bestscore_pos.y = 520;
	snaky = graphics->LoadImage("Menu/SnakyLabel.png", scaleFactor);
	snaky_pos.x = 200 * scaleFactor / game->GetScaleFactor();
	snaky_pos.y = 100 * scaleFactor / game->GetScaleFactor();
}

void MenuScreen::Update(float sec){
	graphics->Clear(0.0f, 0.25f, 0.25f);
	UpdateSun(sec);
	graphics->DrawImage(background_pos, background);

	play_btn->Update();
	music_btn->Update();
	sound_btn->Update();

	graphics->DrawImage(snaky_pos, snaky);
	graphics->DrawImage(bestscore_pos, bestscore);
	score_texter->DrawText(510, 490, to_string(score));
}

void MenuScreen::UpdateSun(float sec){
	static float startAngle = 0;
	static float deltaAngle = 0;
	static float deltaTime = 0;
	static float lerpVal = 0;

	if(input->HaveInput() && startAngle == 0){
		float sun_tangens = (sun_pos.x - input->GetInput().x) / (sun_pos.y - input->GetInput().y);
		startAngle = atan(sun_tangens);
		startAngle = (float)(startAngle * 180.f / PI);
	}

	if(!input->HaveInput() && startAngle != 0){
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

	if(input->HaveInput()){
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
		sun_w = lerp(sun_w, 5.f, lerpVal);
	}
}

void MenuScreen::OnPlayClick(){

}

void MenuScreen::OnMusicClick(){

}

void MenuScreen::OnSoundClick(){

}