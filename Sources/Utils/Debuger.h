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

#include "Game.h"
#ifndef C3D
#include "Texter.h"
#endif

namespace cross{

class Debuger{
public:
	static void StartCheckTime();
	static void StopCheckTime(string label);

	Debuger(Game* game);
	~Debuger();

	void Display(float sec);
	void SetUpdateTime(float sec);
	void EnableScreenDebug();
	void EnableConsoleDebug();
	void EnableTouches();
private:
	Game* game;
	Input* input;
#ifndef C3D
	Texter* texter;
	Image* touch_pointer;
#endif

	float update_time;
	float update_sum;
	int update_counter;

	float render_time;
	float render_sum;
	int render_counter;

	float time;
	float next_display;

	bool screen_debug;
	bool console_debug;
	bool touches;
};
    
}
