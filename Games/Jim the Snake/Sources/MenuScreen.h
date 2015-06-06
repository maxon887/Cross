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

#include "Screen.h"
#include "Texter.h"
#include "Button.h"
#include "ToggleButton.h"

#include <vector>

class MenuScreen : public Screen{
public:
	MenuScreen(Game* game);
	void Start();
	void Update(float sec);
	~MenuScreen();
private:
	Image* background;
	PointX background_pos;
	//sun
	Image* sun;
	PointX sun_pos;
	float sun_angle;
	float sun_w;
	vector<RectX> dead_areas;
	
	Texter* score_texter;
	int score;
	Image* bestscore;
	Image* snaky;
	PointX snaky_pos;

	Button* play_btn;
	ToggleButton* music_btn;
	ToggleButton* sound_btn;
	Audio* menu_music;

	void CreateDeadAreas();
	void UpdateSun(float sec);
	void OnPlayClick();
	void OnMusicClick();
	void OnSoundClick();
};