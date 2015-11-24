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

MenuScreen::MenuScreen(JimTheSnake* game) {
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
	lerp_val = 0;
	start_angle = 0;
	delta_angle = 0;
	delta_time = 0;
	transition = false;
	onLeft = true;
	going_on_game = false;
	touch_down = false;
	transitionLerp = 0.f;
	input->KeyPressed.Clear();
	input->ActionDown.Clear();
	input->ActionDown += MakeDelegate(this, &MenuScreen::ActionDownHandler);
	input->ActionMove.Clear();
	input->ActionMove += MakeDelegate(this, &MenuScreen::ActionMoveHandler);
	input->ActionUp.Clear();
	input->ActionUp += MakeDelegate(this, &MenuScreen::ActionUpHandler);
	//image loading
	background			= graphics->LoadImage("Menu/Background.png");
	sun					= graphics->LoadImage("Menu/Sun.jpg");
	bestscore			= graphics->LoadImage("Menu/BestScoreLabel.png", game->GetScaleFactor() * 1.2f);
	jimthesnake			= graphics->LoadImage("Menu/JimTheSnakeLabel.png");
	arrows_up			= graphics->LoadImage("Menu/ArrowsUp.png", game->GetScaleFactor() * 1.2f);
	arrows_down			= graphics->LoadImage("Menu/ArrowsDown.png", game->GetScaleFactor() * 1.2f);
	slide_up			= graphics->LoadImage("Menu/SlideUp.png", game->GetScaleFactor() * 1.2f);
	slide_down			= graphics->LoadImage("Menu/SlideDown.png", game->GetScaleFactor() * 1.2f);
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
	Image* restoreUp	= graphics->LoadImage("Menu/RestorePurchaseUp.png", game->GetScaleFactor() * 1.2f);
	Image* restoreDown  = graphics->LoadImage("Menu/RestorePurchaseDown.png", game->GetScaleFactor() * 1.2f);
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
	sun_pos.y = game->GetHeight() / 5.f * 4.2f;
	jimthesnake_pos.x = game->GetWidth() / 2.f;
	jimthesnake_pos.y = 120.f * scaleFactor / game->GetScaleFactor();
	play_btn_pos.x = game->GetWidth() / 3.f * 2.f;
	play_btn_pos.y = game->GetHeight() / 3.f * 2.f;
	settings_pos.x = game->GetWidth() - settingsUp->GetWidth() * 0.8f;
	settings_pos.y = game->GetHeight() - settingsUp->GetHeight() * 0.8f;
	float offset = bestscore->GetHeight() / 2.f;
	best_lb_pos.x = bestscore->GetWidth() / 2.f + 30.f;
	best_lb_pos.y = game->GetHeight() - offset - 20.f;
	best_tx_pos.x = bestscore->GetWidth() / 2.f + 170.f + 30.f;
	best_tx_pos.y = game->GetHeight() - offset - 55.f;
	back_pos.x = backUp->GetWidth() * 0.8f + game->GetWidth();
	back_pos.y = game->GetHeight() - backUp->GetHeight() * 0.8f;
#ifdef IOS
	float baseOffset = 0;// game->GetHeight() / 30.f;
#else
	float baseOffset = game->GetWidth() / 30.f;
#endif
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
	restore_pos.x = remove_ads_pos.x;
	restore_pos.y = remove_ads_pos.y + restoreUp->GetHeight() / 2.f + removeAdsUp->GetHeight() / 2.f + 50.f;

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
	play_btn->Clicked += MakeDelegate(this, &MenuScreen::OnPlayClick);
	settings_btn = new Button(game, settingsUp, settingsDown);
	settings_btn->Clicked += MakeDelegate(this, &MenuScreen::OnSettingsClick);
	back_btn = new Button(game, backUp, backDown);
	back_btn->Clicked += MakeDelegate(this, &MenuScreen::OnSettingsClick);
	music_btn = new Button(game, musicUp, musicDown);
	music_btn->Clicked += MakeDelegate(this, &MenuScreen::OnMusicClick);
	music_chk = new ToggleButton(game, check1, uncheck1);
	music_chk->Clicked += MakeDelegate(this, &MenuScreen::OnMusicClick);
	sounds_btn = new Button(game, soundsUp, soundsDown);
	sounds_btn->Clicked += MakeDelegate(this, &MenuScreen::OnSoundClick);
	sounds_chk = new ToggleButton(game, check2, uncheck2);
	sounds_chk->Clicked += MakeDelegate(this, &MenuScreen::OnSoundClick);
	control_btn = new Button(game, controlUp, controlDown);
	control_btn->Clicked += MakeDelegate(this, &MenuScreen::OnControlClick);
	control_chk = new Button(game, arrows_up->GetWidth(), arrows_up->GetHeight());
	control_chk->Clicked += MakeDelegate(this, &MenuScreen::OnControlClick);
	remove_ads_btn = new Button(game, removeAdsUp, removeAdsDown);
	remove_ads_btn->Clicked += MakeDelegate(this, &MenuScreen::OnRemoveAdsClick);
	restore_btn = new Button(game, restoreUp, restoreDown);
	restore_btn->Clicked += MakeDelegate(this, &MenuScreen::OnRestoreClick);
	//misc
	score_texter = new Texter(game, "Menu/NumbersYellow.png", 65.f, 76.f, 10, 1, 48);
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
	input->KeyPressed += MakeDelegate(this, &MenuScreen::KeyPressedHandler);
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
	static Point restorePos			= restore_pos;
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
	restore_btn->SetLocation(restorePos);

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
	case SLIDE:
		if(control_chk->IsPressed()){
			graphics->DrawImage(controlValuePos, slide_down);
		}else{
			graphics->DrawImage(controlValuePos, slide_up);
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
			restorePos.x = Lerp(restorePos.x, restore_pos.x - game->GetWidth(), transitionLerp);
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
			restorePos.x = Lerp(restorePos.x, restore_pos.x, transitionLerp);
		}
	}
	//DrawDeadAreas();
	settings_btn->Update();
	back_btn->Update();
	music_btn->Update();
	sounds_btn->Update();
	control_btn->Update();
	if(!game->IsPurchased()){
		remove_ads_btn->Update();
#ifdef IOS
		restore_btn->Update();
#endif
	}
	music_chk->Update();
	sounds_chk->Update();
	control_chk->Update();
	play_btn->Update();
	if(going_on_game){
		game->SetScreen(new GameScreen(game));
	}
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
	if(touch_down){
		float sun_tangens = (sun_pos.x - input_pos.x) / (sun_pos.y - input_pos.y);
		delta_angle = atan(sun_tangens);
		delta_angle = (float)(delta_angle * 180.f / PI);
		delta_angle = delta_angle - start_angle;
		graphics->Rotate(sun, sun_angle + delta_angle);
		graphics->DrawImage(sun_pos, sun);
		delta_time += sec;
	}else{
		graphics->Rotate(sun, sun_angle);
		graphics->DrawImage(sun_pos, sun);
		sun_angle += sec * sun_w;
		if(lerp_val < 1)
			lerp_val += 0.05f * sec;
		else 
			lerp_val = 1;
		sun_w = Lerp(sun_w, 5.f, lerp_val);
	}
}

void MenuScreen::OnPlayClick(){
	going_on_game = true;
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
	game->SetControl((Control)((game->GetControl() + 1) % 2));
}

void MenuScreen::OnRemoveAdsClick(){
	Commercial* comm = game->GetCommercial();
	if(comm != NULL){
		comm->Purchase();
	}
}

void MenuScreen::OnRestoreClick(){
    Commercial* comm = game->GetCommercial();
    if(comm != NULL){
        comm->Restore();
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
	graphics->ReleaseImage(arrows_up);
	graphics->ReleaseImage(arrows_down);
	graphics->ReleaseImage(slide_up);
	graphics->ReleaseImage(slide_down);
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
	delete restore_btn;
	delete menu_music;
	delete btn_push;
	delete btn_pull;
	delete score_texter;
}

void MenuScreen::KeyPressedHandler(Key key){
	if(key == Key::BACK){
		if(onLeft){
			launcher->PromtToExit();
		}else{
			OnSettingsClick();
		}
	}
}

void MenuScreen::ActionDownHandler(Point pos){
	if(onLeft){
		for(Rect dead : dead_areas_left){
			if(PointInRect(pos, dead)){
				return;
			}
		}
	}else{
		for(Rect dead : dead_areas_right){
			if(PointInRect(pos, dead)){
				return;
			}
		}
	}
	float sun_tangens = (sun_pos.x - pos.x) / (sun_pos.y - pos.y);
	start_angle = atan(sun_tangens);
	start_angle = (float)(start_angle * 180. / PI);
	input_pos = pos;
	touch_down = true;
}

void MenuScreen::ActionMoveHandler(Point pos){
	input_pos = pos;
}

void MenuScreen::ActionUpHandler(Point pos){
	sun_angle = sun_angle + delta_angle;
	if(delta_angle < -90){
		delta_angle += 180.f;
	}
	if(delta_angle > 90){
		delta_angle -= 180.f;
	}
	if(delta_time != 0){
		sun_w = delta_angle / delta_time;
	}
	start_angle = 0;
	delta_time = 0;
	lerp_val = 0;
	touch_down = false;
}
