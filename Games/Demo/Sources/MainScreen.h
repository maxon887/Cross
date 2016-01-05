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

#include "Graphics2D.h"
#include "Screen.h"
#include "Point.h"
#include "Texter.h"
#include "Debuger.h"
#include "Button.h"
#include "ToggleButton.h"

using namespace cross;

enum GoingScreen{
	NO_SCREEN,
	ANIMATION,
	AUDIO,
	PRIMITIVES,
	MISC
};

class MainScreen : public Screen{
public:
	void Start();
	void Update(float sec);

	void OnAnimationClick();
	void OnAudioClick();
	void OnPrimitivesClick();
	void OnMiscClick();
	~MainScreen();
private:
	Image* x_img;
	Button* animation_btn;
	Button* audio_btn;
	Button* primitives_btn;
	Button* misc_btn;

	Texter* texter;
	int start_count;
	GoingScreen going_screen;
};
