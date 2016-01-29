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

#include <chrono>

typedef chrono::time_point<chrono::high_resolution_clock> CrossTime;

namespace cross{

class Debuger{
public:
	Debuger();
	~Debuger();

	void StartCheckTime();
	void StopCheckTime(string label);
	void Display(float sec);
	void SetUpdateTime(float sec);
	void EnableScreenDebug();
	void EnableConsoleDebug();
	void EnableTouches();
	float GetFPS();

	void OnActionDown(Vector2D pos);
	void OnActionUp(Vector2D pos);
	void OnActionMove(Vector2D pos);

	Texter* GetTexter();

private:
	Texter* texter;
	Sprite* touch_pointer;
	CRArray<CrossTime> times;

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
	Vector2D touch_pos;
	bool touch_down;
};
    
}
