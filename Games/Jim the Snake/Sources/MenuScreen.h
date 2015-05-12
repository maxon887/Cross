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

#include "Graphics.h"
#include "Screen.h"
#include "Image.h"
#include "Texter.h"
#include "Button.h"
#include "ToggleButton.h"
#include "SnakyGame.h"
#include "GameScreen.h"

#include <math.h>

class MenuScreen : public Screen{
public:
	MenuScreen(Game* game);
	void Start();
	void Update(float sec);
private:
	Image* background;
	PointX background_pos;
	Image* sun;
	PointX sun_pos;
	float sun_angle;
	float sun_w;
	vector<RectX> dead_areas;
	
	Texter* score_texter;
	int score;
	Image* bestscore;
	PointX bestscore_pos;
	Image* snaky;
	PointX snaky_pos;

	Button* play_btn;
	ToggleButton* music_btn;
	ToggleButton* sound_btn;

	void UpdateSun(float sec);
	void OnPlayClick();
	void OnMusicClick();
	void OnSoundClick();
};