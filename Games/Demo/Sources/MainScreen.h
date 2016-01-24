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
#include "Cross.h"
#include "Screen.h"
#include "Button.h"
#include "ToggleButton.h"
#include "Menu.h"

using namespace cross;

enum GoingScreen{
	NO_SCREEN,
	ANIMATION,
	AUDIO,
	PRIMITIVES,
	MISC,
	TEST_NA_PIDORA
};

class MainScreen : public Screen{
public:
	void Start();
	void Update(float sec);

	void OnAnimationClick();
	void OnAudioClick();
	void OnPrimitivesClick();
	void OnMiscClick();
	void OnTestNaPidoraClick();
	~MainScreen();
private:
	Menu* menu;
	/*
	Button* audio_btn;
	Button* primitives_btn;
	Button* test_label_button;*/

	GoingScreen going_screen;
};
