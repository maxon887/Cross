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
	menu_music = NULL;
	btn_pull = NULL;
	btn_push = NULL;
	sun_w = 5.f;
	sun_angle = 0;
	score = 0;
	transition = false;
	onLeft = true;
	transitionLerp = 0.f;
	//image loading
	background			= graphics->LoadImage("Menu/Background.png");
	sun					= graphics->LoadImage("Menu/Sun.jpg");
	bestscore			= graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	jimthesnake			= graphics->LoadImage("Menu/JimTheSnakeLabel.png");
	arrows_up			= graphics->LoadImage("Menu/ArrowsUp.png", game->GetScaleFactor() * 1.2f);
	arrows_down			= graphics->LoadImage("Menu/ArrowsDown.png", game->GetScaleFactor() * 1.2f);
	pointer_up			= graphics->LoadImage("Menu/PointerUp.png", game->GetScaleFactor() * 1.2f);
	pointer_down		= graphics->LoadImage("Menu/PointerDown.png", game->GetScaleFactor() * 1.2f);
	none_up				= graphics->LoadImage("Menu/NoneUp.png", game->GetScaleFactor() * 1.2f);
	none_down			= graphics->LoadImage("Menu/NoneDown.png", game->GetScaleFactor() * 1.2f);
	Image* playUp		= graphics->LoadImage("Menu/PlayButtonUp.png");
	Image* playDown		= graphics->LoadImage("Menu/PlayButtonDown.png");
	Image* settingsUp	= graphics->LoadImage("Menu/SettingsUp.png", game->GetScaleFactor() * 1.2f);
	Image* settingsDown = graphics->LoadImage("Menu/SettingsDown.png", game->GetScaleFactor() * 1.2f);
	Image* backUp		= graphics->LoadImage("Menu/BackUp.png", game->GetScaleFactor() * 1.2f);
	Image* backDown		= graphics->LoadImage("Menu/BackDown.png", game->GetScaleFactor() * 1.2f);
	Image* musicUp		= graphics->LoadImage("Menu/MusicUp.png", game->GetScaleFactor() * 1.2f);
	Image* musicDown	= graphics->LoadImage("Menu/MusicDown.png", game->GetScaleFactor() * 1.2f);
	Image* soundsUp		= graphics->LoadImage("Menu/SoundsUp.png", game->GetScaleFactor() * 1.2f);
	Image* soundsDown   = graphics->LoadImage("Menu/SoundsDown.png", game->GetScaleFactor() * 1.2f);
	Image* controlUp	= graphics->LoadImage("Menu/ControlUp.png", game->GetScaleFactor() * 1.2f);
	Image* controlDown  = graphics->LoadImage("Menu/ControlDown.png", game->GetScaleFactor() * 1.2f);
	Image* removeAdsUp	= graphics->LoadImage("Menu/RemoveAdsUp.png", game->GetScaleFactor() * 1.2f);
	Image* removeAdsDown= graphics->LoadImage("Menu/RemoveAdsDown.png", game->GetScaleFactor() * 1.2f);
	Image* check1		= graphics->LoadImage("Menu/Check.png", game->GetScaleFactor() * 1.2f);
	Image* uncheck1		= graphics->LoadImage("Menu/Uncheck.png", game->GetScaleFactor() * 1.2f);
	Image* check2		= graphics->LoadImage("Menu/Check.png", game->GetScaleFactor() * 1.2f);
	Image* uncheck2		= graphics->LoadImage("Menu/Uncheck.png", game->GetScaleFactor() * 1.2f);
	//positioning
	float scaleFactor = 0;
	float imageAspect = background->GetWidth() / 2 / background->GetHeight();
	float deviceAspect = (float)launcher->GetTargetWidth() / launcher->GetTargetHeight();
	if(imageAspect < deviceAspect){
		scaleFactor = launcher->GetTargetWidth() / (background->GetWidth() / 2);
		background_pos.x = game->GetWidth();
	}else{
		scaleFactor = launcher->GetTargetHeight() / background->GetHeight();
        background_pos.x = game->GetWidth();
	}
	background_pos.y = game->GetHeight() / 2.f;
	sun_pos.x = game->GetWidth();
	sun_pos.y = game->GetHeight() / 5 * 4.2;
	jimthesnake_pos.x = game->GetWidth() / 2;
	jimthesnake_pos.y = 120.f * scaleFactor / game->GetScaleFactor();
	play_btn_pos.x = game->GetWidth() / 3 * 2;
	play_btn_pos.y = game->GetHeight() / 3 * 2;
	settings_pos.x = game->GetWidth() - settingsUp->GetWidth() * 0.8f;
	settings_pos.y = game->GetHeight() - settingsUp->GetHeight() * 0.8f;
	float offset = bestscore->GetHeight() / 2;
	best_lb_pos.x = bestscore->GetWidth() / 2 + 30.f;
	best_lb_pos.y = game->GetHeight() - offset - 20.f;
	best_tx_pos.x = bestscore->GetWidth() / 2 + 170.f + 30.f;
	best_tx_pos.y = game->GetHeight() - offset - 55.f;
	back_pos.x = backUp->GetWidth() * 0.8f + game->GetWidth();
	back_pos.y = game->GetHeight() - backUp->GetHeight() * 0.8f;

	float baseOffset = game->GetHeight() / 30.f;
	music_pos.x = musicUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	music_pos.y = (game->GetHeight() / 8.f) * 1.f + baseOffset;
	music_value_pos.x = game->GetWidth() - check1->GetWidth() + game->GetWidth();
	music_value_pos.y = (game->GetHeight() / 8.f) * 1.f + baseOffset;
	sounds_pos.x = soundsUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	sounds_pos.y = (game->GetHeight() / 8.f) * 2.f + baseOffset;
	sounds_value_pos.x = game->GetWidth() - check1->GetWidth() + game->GetWidth();
	sounds_value_pos.y = (game->GetHeight() / 8.f) * 2.f + baseOffset;
	control_pos.x = controlUp->GetWidth() / 2.f + 60.f + game->GetWidth();
	control_pos.y = (game->GetHeight() / 8.f) * 3.f + baseOffset;
	control_value_pos.x = game->GetWidth() / 3.f * 2.f + game->GetWidth();
	control_value_pos.y = (game->GetHeight() / 8.f) * 3.75f + baseOffset;
	remove_ads_pos.x = game->GetWidth() / 3.f + game->GetWidth();
	remove_ads_pos.y = (game->GetHeight() / 8.f) * 5.f + baseOffset;

	//scaling
	graphics->ScaleImage(background, scaleFactor);
	graphics->ScaleImage(sun, scaleFactor);
	graphics->ScaleImage(jimthesnake, scaleFactor * 0.9f);
	//music
	menu_music = new Audio("Menu/MenuMusic.mp3", true, true);
	btn_push = new Audio("ButtonPush.wav", false, false);
	btn_pull = new Audio("ButtonPull.wav", false, false);
	//buttons creation
	play_btn = new Button(game, play_btn_pos, playUp, playDown);
	play_btn->RegisterCallback(bind(&MenuScreen::OnPlayClick, this));
	settings_btn = new Button(game, settingsUp, settingsDown);
	settings_btn->RegisterCallback(bind(&MenuScreen::OnSettingsClick, this));
	back_btn = new Button(game, backUp, backDown);
	back_btn->RegisterCallback(bind(&MenuScreen::OnSettingsClick, this));
	music_btn = new Button(game, musicUp, musicDown);
	music_btn->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));
	music_chk = new ToggleButton(game, check1, uncheck1);
	music_chk->RegisterCallback(bind(&MenuScreen::OnMusicClick, this));
	sounds_btn = new Button(game, soundsUp, soundsDown);
	sounds_btn->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	sounds_chk = new ToggleButton(game, check2, uncheck2);
	sounds_chk->RegisterCallback(bind(&MenuScreen::OnSoundClick, this));
	control_btn = new Button(game, controlUp, controlDown);
	control_btn->RegisterCallback(bind(&MenuScreen::OnControlClick, this));
	control_chk = new Button(game, pointer_up->GetWidth(), pointer_up->GetHeight());
	control_chk->RegisterCallback(bind(&MenuScreen::OnControlClick, this));
	remove_ads_btn = new Button(game, removeAdsUp, removeAdsDown);
	remove_ads_btn->RegisterCallback(bind(&MenuScreen::OnRemoveAdsClick, this));
	//misc
	score_texter = new Texter(game, "NumbersYellow.png", 65.f, 76.f, 10, 1, 48);
	score = game->BestScore();
	sounds_chk->SetState(game->IsSoundEnabled());
	music_chk->SetState(game->IsMusicEnabled());
	if(game->IsMusicEnabled()){
		menu_music->Play();
	}
	if(game->IsSoundEnabled()){
		SetupButtonSounds(btn_push, btn_pull);
	}
	CreateDeadAreas();
}

void MenuScreen::Update(float sec){
	graphics->Clear(1.f, 0.352f, 0.082f);
	static Point backgroundPos		= background_pos;
	static Point jimthesnakePos		= jimthesnake_pos;
	static Point playBtnPos			= play_btn_pos;
	static Point settingsPos		= settings_pos;
	static Point backPos			= back_pos;
	static Point sunPos				= sun_pos;
	static Point musicPos			= music_pos;
	static Point soundsPos			= sounds_pos;
	static Point bestLbPos			= best_lb_pos;
	static Point bestTxPos			= best_tx_pos;
	static Point controlPos			= control_pos;
	static Point removeAdsPos		= remove_ads_pos;
	static Point musicValuePos		= music_value_pos;
	static Point soundsValuePos		= sounds_value_pos;
	static Point controlValuePos	= control_value_pos;
	UpdateSun(sec, sunPos);
	play_btn->SetLocation(playBtnPos);
	settings_btn->SetLocation(settingsPos);
	back_btn->SetLocation(backPos);
	music_btn->SetLocation(musicPos);
	sounds_btn->SetLocation(soundsPos);
	control_btn->SetLocation(controlPos);
	remove_ads_btn->SetLocation(removeAdsPos);
	music_chk->SetLocation(musicValuePos);
	sounds_chk->SetLocation(soundsValuePos);
	control_chk->SetLocation(controlValuePos);

	graphics->DrawImage(backgroundPos, background);
	graphics->DrawImage(jimthesnakePos, jimthesnake);
	graphics->DrawImage(bestLbPos, bestscore);
	score_texter->DrawText(bestTxPos, to_string(score));
	switch (game->GetControl()){
	case ARROWS:
		if(control_chk->IsPressed()){
			graphics->DrawImage(controlValuePos, arrows_down);
		}else{
			graphics->DrawImage(controlValuePos, arrows_up);
		}
		break;
	case POINTER:
		if(control_chk->IsPressed()){
			graphics->DrawImage(controlValuePos, pointer_down);
		}else{
			graphics->DrawImage(controlValuePos, pointer_up);
		}
		break;
	case NONE:
		if(control_chk->IsPressed()){
			graphics->DrawImage(controlValuePos, none_down);
		}else{
			graphics->DrawImage(controlValuePos, none_up);
		}
		break;
	default:
		break;
	}

	if(transition){
		if(transitionLerp < 1) {
			transitionLerp += 0.15f * sec;
		} else {
			transitionLerp = 1;
		}
		if(!onLeft){
			sunPos.x = Lerp(sunPos.x, sun_pos.x - game->GetWidth(), transitionLerp);
			backgroundPos.x = Lerp(backgroundPos.x, background_pos.x - game->GetWidth(), transitionLerp);
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
	//DrawDeadAreas();
	if(input->HaveKey() && input->GetKey() == Key::BACK){
		if(onLeft){
			launcher->PromtToExit();
		}else{
			OnSettingsClick();
		}
	}
	settings_btn->Update();
	back_btn->Update();
	music_btn->Update();
	sounds_btn->Update();
	control_btn->Update();
	if(!game->IsPurchased()){
		remove_ads_btn->Update();
	}
	music_chk->Update();
	sounds_chk->Update();
	control_chk->Update();
	play_btn->Update();
}

void MenuScreen::CreateDeadAreas(){
	Rect r = play_btn->GetRect();
	dead_areas_left.push_back(r);
	r.x = 0;
	r.y = play_btn->GetCenter().y;
	r.width = game->GetWidth();
	r.height = game->GetHeight() - r.y;
	dead_areas_left.push_back(r);
	r.x = 0;
	r.y = 570;
	r.width = 250;
	r.height = game->GetHeight() - r.y;
	dead_areas_left.push_back(r);

	r.x = 0;
	r.y = 0;
	r.width = game->GetWidth();
	r.height = game->GetHeight();
	dead_areas_right.push_back(r);
}

void MenuScreen::UpdateSun(float sec, Point sun_pos){
	static float startAngle = 0;
	static float deltaAngle = 0;
	static float deltaTime = 0;
	static float lerpVal = 0;
	bool touchInDeadArea = false;

	if(input->HaveInput() && startAngle == 0){
		if(onLeft){
			//for(unsigned int i = 0; i < dead_areas_left.size(); i++){
			for(Rect dead : dead_areas_left){
				if(PointInRect(input->GetInput(), dead)){
					touchInDeadArea = true;
					break;
				}
			}
		}else{
			for(Rect dead : dead_areas_right){
				if(PointInRect(input->GetInput(), dead)){
					touchInDeadArea = true;
					break;
				}
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
	music_chk->SetState(game->IsMusicEnabled());
	if(game->IsMusicEnabled()){
		menu_music->Play();
	}else{
		menu_music->Stop();
	}
}

void MenuScreen::OnSoundClick(){
	game->SetSoundEnabled(!game->IsSoundEnabled());
	sounds_chk->SetState(game->IsSoundEnabled());
	if(game->IsSoundEnabled()){
		SetupButtonSounds(btn_push, btn_pull);
	}else{
		SetupButtonSounds(NULL, NULL);
	}
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
	Commercial* comm = game->GetCommercial();
	if(comm != NULL){
		comm->Purchase();
	}
}

void MenuScreen::DrawDeadAreas(){
	if(onLeft){
		for(Rect dead : dead_areas_left){
			graphics->DrawRect(dead, Color::Red);
		}
	}else{
		for(Rect dead : dead_areas_right){
			graphics->DrawRect(dead, Color::Red);
		}
	}
}

void MenuScreen::SetupButtonSounds(Audio* btn_push, Audio* btn_pull){
	play_btn->SetSounds(btn_push, btn_pull);
	settings_btn->SetSounds(btn_push, btn_pull);
	back_btn->SetSounds(btn_push, btn_pull);
	music_btn->SetSounds(btn_push, btn_pull);
	music_chk->SetSounds(btn_push, btn_pull);
	sounds_btn->SetSounds(btn_push, btn_pull);
	sounds_chk->SetSounds(btn_push, btn_pull);
	control_btn->SetSounds(btn_push, btn_pull);
	control_chk->SetSounds(btn_push, btn_pull);
	remove_ads_btn->SetSounds(btn_push, btn_pull);
}

MenuScreen::~MenuScreen(){
	graphics->ReleaseImage(sun);
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(bestscore);
	graphics->ReleaseImage(jimthesnake);
	//graphics->ReleaseImage(check);
	//graphics->ReleaseImage(uncheck);
	graphics->ReleaseImage(arrows_up);
	graphics->ReleaseImage(arrows_down);
	graphics->ReleaseImage(pointer_up);
	graphics->ReleaseImage(pointer_down);
	graphics->ReleaseImage(none_up);
	graphics->ReleaseImage(none_down);
	delete play_btn;
	delete settings_btn;
	delete back_btn;
	delete music_btn;
	delete music_chk;
	delete sounds_btn;
	delete sounds_chk;
	delete control_btn;
	delete control_chk;
	delete remove_ads_btn;
	delete menu_music;
	delete btn_push;
	delete btn_pull;
	delete score_texter;
}
