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
	
#pragma once

#include "JimTheSnake.h"
#include "Texter.h"
#include "Button.h"
#include "ToggleButton.h"
#include "Commercial.h"

#include <vector>

class MenuScreen : public Screen{
public:
	MenuScreen(JimTheSnake* game);
	~MenuScreen();

	void Start();
	void Update(float sec);
private:
	JimTheSnake* game;
	Image* background;
	Point background_pos;
	//sun
	Image* sun;
	Point sun_pos;
	float sun_angle;
	float sun_w;
	vector<Rect> dead_areas_left;
	vector<Rect> dead_areas_right;
	
	Image* bestscore;
	Texter* score_texter;
	Point best_lb_pos;
	Point best_tx_pos;
	int score;
	Image* jimthesnake;
	Point jimthesnake_pos;

	Button* play_btn;
	Point	play_btn_pos;
	Button* settings_btn;
	Point	settings_pos;

	Button* back_btn;
	Point	back_pos;

	Button* music_btn;
	ToggleButton* music_chk;
	Point   music_pos;
	Point   music_value_pos;
	bool    music_value;

	Button* sounds_btn;
	ToggleButton* sounds_chk;
	Point	sounds_pos;
	Point   sounds_value_pos;
	bool    sounds_value;

	Audio* btn_push;
	Audio* btn_pull;

	Button* control_btn;
	Button* control_chk;
	Image*  arrows_up;
	Image*	arrows_down;
	Image*  slide_up;
	Image*  slide_down;
	Point	control_pos;
	Point   control_value_pos;
	Control control_value;

	Button* remove_ads_btn;
	Point   remove_ads_pos;
	Button* restore_btn;
	Point	restore_pos;
	Audio* menu_music;
	bool transition;
	float transitionLerp;
	bool onLeft;

	void KeyPressedHandler(Key key);
	void CreateDeadAreas();
	void DrawDeadAreas();
	void SetupButtonSounds(Audio* push, Audio* pull);
	void UpdateSun(float sec, Point sun_pos);
	void OnPlayClick();
	void OnMusicClick();
	void OnSoundClick();
	void OnSettingsClick();
	void OnControlClick();
	void OnRemoveAdsClick();
	void OnRestoreClick();
};