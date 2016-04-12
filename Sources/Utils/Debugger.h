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
#include "Input.h"

#include <chrono>

typedef chrono::time_point<chrono::high_resolution_clock> CrossTime;

namespace cross{

class Debugger{
public:
	static Debugger* Instance();
	static void Release();

	void StartCheckTime();
	void StopCheckTime(string label);
	void Display(float sec);
	void SetUpdateTime(float sec);
	void ScreenDebug(bool enable);
	void EnableInputDebug();
	void ConsoleDebug(bool enable);
	float GetFPS();

private:
	static Debugger* instance;

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
	bool input_debug;
	bool console_debug;
	bool touches;
	Vector2D touch_pos;
	bool touch_down;

	Debugger();
	~Debugger();

	void OnActionDown(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnActionMove(Input::Action action);
};
    
}
