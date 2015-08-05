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
	Image* background_left;
	Image* background_right;
	Point background_pos;
	Point background_left_pos;
	Point background_right_pos;
	//sun
	Image* sun;
	Point sun_pos;
	float sun_angle;
	float sun_w;
	vector<Rect> dead_areas;
	
	Texter* score_texter;
	int score;
	Image* bestscore;
	Image* jimthesnake;
	Point jimthesnake_pos;

	Button* play_btn;
	Point play_btn_pos;
	Button* settings_btn;
	Point settings_pos;
	Button* back_btn;
	Point back_pos;
	ToggleButton* music_btn;
	ToggleButton* sound_btn;
	Audio* menu_music;
	bool transition;
	float transitionLerp;
	bool onLeft;

	void CreateDeadAreas();
	void UpdateSun(float sec, Point sun_pos);
	void OnPlayClick();
	void OnMusicClick();
	void OnSoundClick();
	void OnSettingsClick();
};