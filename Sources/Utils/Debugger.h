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
		FPS,
		UPDATE_TIME,
		CPU_TIME,
		RUN_TIME,
		INPUT,
		NONE
	};

	static Debugger* Instance();
	static void Release();

	void Update(float micro);

	void SetTimeCheck();
	float GetTimeCheck();
	void SetCPUTime(float sec);
	float GetFPS() const;

private:
	static Debugger* instance;

	Array<U64> time_checks			= Array<U64>();

	Array<bool> params				= Array<bool>(Parameter::NONE, false);

	float cpu_time					= 0;
	float cpu_sum					= 0;
	int cpu_counter					= 0;

	float update_time				= 0;
	float update_sum				= 0;
	int update_counter				= 0;

	bool touches					= false;
	Vector2D touch_pos				= Vector2D(0);
	bool touch_down					= false;

	Debugger();
	~Debugger();

	void OnActionDown(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnActionMove(Input::Action action);
};
    
}
