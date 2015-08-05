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
	transition = false;
	onLeft = true;
	transitionLerp = 0.f;
	//image loading
	background = graphics->LoadImage("Menu/Background.png");
	background_left = graphics->LoadImage("Menu/BackgroundRight.png");
	background_right = graphics->LoadImage("Menu/BackgroundRight.png");
	sun = graphics->LoadImage("Menu/Sun.jpg");
	bestscore = graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	jimthesnake = graphics->LoadImage("Menu/JimTheSnakeLabel.png");
	on = graphics->LoadImage("Menu/OnLabel.png", game->GetScaleFactor() * 1.2f);
	off = graphics->LoadImage("Menu/OffLabel.png", game->GetScaleFactor() * 1.2f);
	arrows = graphics->LoadImage("Menu/ArrowsLabel.png", game->GetScaleFactor() * 1.2f);
	pointer = graphics->LoadImage("Menu/PointerLabel.png", game->GetScaleFactor() * 1.2f);
	none = graphics->LoadImage("Menu/NoneLabel.png", game->GetScaleFactor() * 1.2f);
	Image* playUp = graphics->LoadImage("Menu/PlayButtonUp.png");
	Image* playDown = graphics->LoadImage("Menu/PlayButtonDown.png");
	//Image* soundUp = graphics->LoadImage("Menu/SoundUp.png", game->GetScaleFactor() * 1.2f);
	//Image* soundDown = graphics->LoadImage("Menu/SoundDown.png", game->GetScaleFactor() * 1.2f);
	//Image* musicUp = graphics->LoadImage("Menu/MusicUp.png", game->GetScaleFactor() * 1.2f);
	//Image* musicDown = graphics->LoadImage("Menu/MusicDown.png", game->GetScaleFactor() * 1.2f);
	Image* settingsUp = graphics->LoadImage("Menu/SettingsUp.png", game->GetScaleFactor() * 1.2f);
	Image* settingsDown = graphics->LoadImage("Menu/SettingsDown.png", game->GetScaleFactor() * 1.2f);
	Image* backUp = graphics->LoadImage("Menu/BackUp.png", game->GetScaleFactor() * 1.2f);
	Image* backDown = graphics->LoadImage("Menu/BackDown.png", game->GetScaleFactor() * 1.2f);
	Image* musicUp = graphics->LoadImage("Menu/MusicLabel.png", game->GetScaleFactor() * 1.2f);
	Image* soundsUp = graphics->LoadImage("Menu/SoundsLabel.png", game->GetScaleFactor() * 1.2f);
	Image* controlUp = graphics->LoadImage("Menu/ControlLabel.png", game->GetScaleFactor() * 1.2f);
	Image* removeAdsUp = graphics->LoadImage("Menu/RemoveAdsLabel.png", game->GetScaleFactor() * 1.2f);
	//positioning
	float scaleFactor = 0;
	float imageAspect = background_left->GetWidth() / background_left->GetHeight();
	float deviceAspect = (float)launcher->GetTargetWidth() / launcher->GetTargetHeight();
	if(imageAspect < deviceAspect){
		scaleFactor = launcher->GetTargetWidth() / background_left->GetWidth();
		background_pos.x = game->GetWidth();
	}else{
		scaleFactor = launcher->GetTargetHeight() / background_left->GetHeight();
		background_pos.x = (game->GetWidth() + background->GetWidth() / 2 * (1 + scaleFactor)) / 2;
	}
	sun_pos.x = 400.f * scaleFactor / game->GetScaleFactor();
	sun_pos.y = 580.f * scaleFactor / game->GetScaleFactor();
	jimthesnake_pos.x = 200.f * scaleFactor / game->GetScaleFactor();
	jimthesnake_pos.y = 120.f * scaleFactor / game->GetScaleFactor();
	background_pos.y = game->GetHeight() / 2.f;
	background_left_pos.x = game->GetWidth() / 2.f;
	background_left_pos.y = game->GetHeight() / 2.f;
	play_btn_pos.x = 270.f * scaleFactor / game->GetScaleFactor();
	play_btn_pos.y = 490.f * scaleFactor / game->GetScaleFactor();
	settings_pos.x = game->GetWidth() - settingsUp->GetWidth() * 0.8f;
	settings_pos.y = game->GetHeight() - settingsUp->GetHeight() * 0.8f;
	float offset = bestscore->GetHeight() / 2;
	best_lb_pos.x = bestscore->GetWidth() / 2 + 30.f;
	best_lb_pos.y = game->GetHeight() - offset - 20.f;
	best_tx_pos.x = bestscore->GetWidth() / 2 + 170.f + 30.f;
	best_tx_pos.y = game->GetHeight() - offset - 55.f;

	back_pos.x = backUp->GetWidth() * 0.8f + game->GetWidth();
	back_pos.y = game->GetHeight() - backUp->GetHeight() * 0.8f;

	//float baseOffset = game->GetHeight() / 20.f;
	float baseOffset = game->GetHeight() / 30.f;
	music_pos.x = musicUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	music_pos.y = (game->GetHeight() / 8.f) * 1.f + baseOffset;
	music_value_pos.x = game->GetWidth() / 4.f * 3.f + game->GetWidth();
	music_value_pos.y = (game->GetHeight() / 8.f) * 1.f + baseOffset;
	sounds_pos.x = soundsUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	sounds_pos.y = (game->GetHeight() / 8.f) * 2.f + baseOffset;
	sounds_value_pos.x = game->GetWidth() / 4.f * 3.f + game->GetWidth();
	sounds_value_pos.y = (game->GetHeight() / 8.f) * 2.f + baseOffset;
	control_pos.x = controlUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	control_pos.y = (game->GetHeight() / 8.f) * 3.f + baseOffset;
	control_value_pos.x = game->GetWidth() / 3.f * 2.f + game->GetWidth();
	control_value_pos.y = (game->GetHeight() / 8.f) * 4.f + baseOffset;
	remove_ads_pos.x = game->GetWidth() / 2.f + game->GetWidth();
	remove_ads_pos.y = (game->GetHeight() / 8.f) * 5.f + baseOffset;

	//scaling
	graphics->ScaleImage(background, scaleFactor);
	graphics->ScaleImage(background_left, scaleFactor);
	graphics->ScaleImage(background_right, scaleFactor);
	graphics->ScaleImage(sun, scaleFactor);
	graphics->ScaleImage(jimthesnake, scaleFactor * 0.9f);
	background_right_pos.x = background_left_pos.x * scaleFactor + background_left->GetWidth() * 2 - 1;
	background_right_pos.y = background_left_pos.y;
	//buttons creation
	play_btn = new Button(game, play_btn_pos, playUp, playDown);
	play_btn->RegisterCallback(bind(&MenuScreen::OnPlayClick, this));
	settings_btn = new Button(game, settings_pos, settingsUp, settingsDown);
	settings_btn->RegisterCallback(bind(&MenuScreen::OnSettingsClick, this));
	back_btn = new Button(game, back_pos, backUp, backDown);
	back_btn->RegisterCallback(bind(&MenuScreen::OnSettingsClick, this));
	music_btn = new Button(game, music_pos, musicUp, NULL);
	music_btn->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));
	sounds_btn = new Button(game, sounds_pos, soundsUp, NULL);
	sounds_btn->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	control_btn = new Button(game, control_pos, controlUp, NULL);
	control_btn->RegisterCallback(bind(&MenuScreen::OnControlClick, this));
	remove_ads_btn = new Button(game, remove_ads_pos, removeAdsUp, NULL);
	remove_ads_btn->RegisterCallback(bind(&MenuScreen::OnRemoveAdsClick, this));
	//misc
	score_texter = new Texter(game, "NumbersYellow.png", 65.f, 76.f, 10, 1, 48);
	score = game->BestScore();
	//sound_btn->SetState(game->IsSoundEnabled());
	menu_music = 0;
	menu_music->Play();
	menu_music = new Audio("Menu/MenuMusic.mp3", true, true);
	//music_btn->SetState(game->IsMusicEnabled());
	if(game->IsMusicEnabled()){
		menu_music->Play();
	}
	CreateDeadAreas();
}

void MenuScreen::Update(float sec){
 	graphics->Clear(0.0f, 0.25f, 0.25f);
	static Point backgroundPos = background_pos;
	static Point jimthesnakePos = jimthesnake_pos;
	static Point playBtnPos = play_btn_pos;
	static Point settingsPos = settings_pos;
	static Point backPos = back_pos;
	static Point backgroundRightPos = background_right_pos;
	static Point sunPos = sun_pos;
	static Point musicPos = music_pos;
	static Point soundsPos = sounds_pos;
	static Point bestLbPos = best_lb_pos;
	static Point bestTxPos = best_tx_pos;
	static Point controlPos = control_pos;
	static Point removeAdsPos = remove_ads_pos;
	static Point musicValuePos = music_value_pos;
	static Point soundsValuePos = sounds_value_pos;
	static Point controlValuePos = control_value_pos;
	UpdateSun(sec, sunPos);
	graphics->DrawImage(backgroundPos, background);
	graphics->DrawImage(jimthesnakePos, jimthesnake);
	play_btn->SetLocation(playBtnPos);
	settings_btn->SetLocation(settingsPos);
	back_btn->SetLocation(backPos);
	music_btn->SetLocation(musicPos);
	sounds_btn->SetLocation(soundsPos);
	control_btn->SetLocation(controlPos);
	remove_ads_btn->SetLocation(removeAdsPos);

	graphics->DrawImage(bestLbPos, bestscore);
	score_texter->DrawText(bestTxPos, to_string(score));
	if(game->IsSoundEnabled()){
		graphics->DrawImage(soundsValuePos, on);
	}else{
		graphics->DrawImage(soundsValuePos, off);
	}
	if(game->IsMusicEnabled()){
		graphics->DrawImage(musicValuePos, on);
	}else{
		graphics->DrawImage(musicValuePos, off);
	}
	switch (game->GetControl()){
	case ARROWS:
		graphics->DrawImage(controlValuePos, arrows);
		break;
	case POINTER:
		graphics->DrawImage(controlValuePos, pointer);
		break;
	case NONE:
		graphics->DrawImage(controlValuePos, none);
		break;
	default:
		break;
	}
	//graphics->DrawImage(Point(bestscore->GetWidth() / 2 + 30.f, game->GetHeight() - offset - 20.f), bestscore);
	//score_texter->DrawText(Point(bestscore->GetWidth() / 2 + 170.f + 30.f, game->GetHeight() - offset - 55.f), to_string(score));
	/*
	for(Rect area : dead_areas){
		graphics->DrawRect(area, Color::Red);
	}*/

	if(transition){
		if(transitionLerp < 1) {
			transitionLerp += 0.15f * sec;
		} else {
			transitionLerp = 1;
		}
		if(!onLeft){
			sunPos.x = Lerp(sunPos.x, sun_pos.x - game->GetWidth(), transitionLerp);
			backgroundPos.x = Lerp(backgroundPos.x, background_pos.x - game->GetWidth(), transitionLerp);
			backgroundRightPos.x = Lerp(backgroundRightPos.x, background_right_pos.x - game->GetWidth(), transitionLerp);
			jimthesnakePos.x = Lerp(jimthesnakePos.x, jimthesnake_pos.x - game->GetWidth(), transitionLerp);
			playBtnPos.x = Lerp(playBtnPos.x, play_btn_pos.x - game->GetWidth(), transitionLerp);
			settingsPos.x = Lerp(settingsPos.x, settings_pos.x - game->GetWidth(), transitionLerp);
			backPos.x = Lerp(backPos.x, back_pos.x - game->GetWidth(), transitionLerp);
			musicPos.x = Lerp(musicPos.x, music_pos.x - game->GetWidth(), transitionLerp);
			soundsPos.x = Lerp(soundsPos.x, sounds_pos.x - game->GetWidth(), transitionLerp);
			bestLbPos.x = Lerp(bestLbPos.x, best_lb_pos.x - game->GetWidth(), transitionLerp);
			bestTxPos.x = Lerp(bestTxPos.x, best_tx_pos.x - game->GetWidth(), transitionLerp);
			controlPos.x = Lerp(controlPos.x, control_pos.x - game->GetWidth(), transitionLerp);
			removeAdsPos.x = Lerp(removeAdsPos.x, remove_ads_pos.x - game->GetWidth(), transitionLerp);
			musicValuePos.x = Lerp(musicValuePos.x, music_value_pos.x - game->GetWidth(), transitionLerp);
			soundsValuePos.x = Lerp(soundsValuePos.x, sounds_value_pos.x - game->GetWidth(), transitionLerp);
			controlValuePos.x = Lerp(controlValuePos.x, control_value_pos.x - game->GetWidth(), transitionLerp);
		}else{
			sunPos.x = Lerp(sunPos.x, sun_pos.x, transitionLerp);
			backgroundPos.x = Lerp(backgroundPos.x, background_pos.x, transitionLerp);
			backgroundRightPos.x = Lerp(backgroundRightPos.x, background_right_pos.x, transitionLerp);
			jimthesnakePos.x = Lerp(jimthesnakePos.x, jimthesnake_pos.x, transitionLerp);
			playBtnPos.x = Lerp(playBtnPos.x, play_btn_pos.x, transitionLerp);
			settingsPos.x = Lerp(settingsPos.x, settings_pos.x, transitionLerp);
			backPos.x = Lerp(backPos.x, back_pos.x, transitionLerp);
			musicPos.x = Lerp(musicPos.x, music_pos.x, transitionLerp);
			soundsPos.x = Lerp(soundsPos.x, sounds_pos.x, transitionLerp);
			bestLbPos.x = Lerp(bestLbPos.x, best_lb_pos.x, transitionLerp);
			bestTxPos.x = Lerp(bestTxPos.x, best_tx_pos.x, transitionLerp);
			controlPos.x = Lerp(controlPos.x, control_pos.x, transitionLerp);
			removeAdsPos.x = Lerp(removeAdsPos.x, remove_ads_pos.x, transitionLerp);
			musicValuePos.x = Lerp(musicValuePos.x, music_value_pos.x, transitionLerp);
			soundsValuePos.x = Lerp(soundsValuePos.x, sounds_value_pos.x, transitionLerp);
			controlValuePos.x = Lerp(controlValuePos.x, control_value_pos.x, transitionLerp);
		}
	}
	settings_btn->Update();
	back_btn->Update();
	music_btn->Update();
	sounds_btn->Update();
	control_btn->Update();
	remove_ads_btn->Update();
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

void MenuScreen::UpdateSun(float sec, Point sun_pos){
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
	game->SetMusicEnabled(!game->IsMusicEnabled());
	if(game->IsMusicEnabled()){
		menu_music->Play();
	}else{
		menu_music->Stop();
	}
}

void MenuScreen::OnSoundClick(){
	game->SetSoundEnabled(!game->IsSoundEnabled());
	//game->SetSoundEnabled(sound_btn->GetState());
}

void MenuScreen::OnSettingsClick(){
	transition = true;
	onLeft = !onLeft;
	transitionLerp = 0;
}

void MenuScreen::OnControlClick(){
	game->SetControl((Control)((game->GetControl() + 1) % 3));
}

void MenuScreen::OnRemoveAdsClick(){

}

MenuScreen::~MenuScreen(){
	graphics->ReleaseImage(background_left);
	graphics->ReleaseImage(background_right);
	graphics->ReleaseImage(sun);
	graphics->ReleaseImage(bestscore);
	graphics->ReleaseImage(jimthesnake);
	delete play_btn;
	delete menu_music;
	delete score_texter;
}
