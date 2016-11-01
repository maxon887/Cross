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

namespace cross{

class Debugger{
public:
	enum Parameter{
		UPDATE_TIME,
		CPU_TIME,
		FPS,
		RUN_TIME,
		INPUT,
		NONE
	};

	static Debugger* Instance();
	static void Release();

	void SetTimeCheck();
	float GetTimeCheck();

	void Update(float micro);

	void SetCPUTime(float sec);
	void EnableDebug(Parameter param);
	void DisableDebug(Parameter param);
	float GetFPS();

private:
	static Debugger* instance;

	CRArray<U64> time_checks;

	Font* debugger_font;

	bool params[Parameter::NONE];

	float cpu_time;
	float cpu_sum;
	int cpu_counter;

	float update_time;
	float update_sum;
	int update_counter;

	bool touches;
	Vector2D touch_pos;
	bool touch_down;

	Debugger();
	~Debugger();

	FastDelegate1<Input::Action, void> action_down_delegate;
	FastDelegate1<Input::Action, void> action_up_delegate;
	FastDelegate1<Input::Action, void> action_move_delegate;

	void OnActionDown(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnActionMove(Input::Action action);
};
    
}
