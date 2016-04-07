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
#include "Input.h"

using namespace cross;

class ScrollScreen : public Screen{
public:

	virtual void Start();
	virtual void Stop();

	virtual float GetWidth();
	virtual float GetHeight();

	void SetWidth(float width);
	void SetHeight(float height);
private:
	float width;
	float height;
	Vector2D touch;

	FastDelegate1<Input::Action, void> action_down_handler;
	FastDelegate1<Input::Action, void> action_move_handler;
	FastDelegate1<Input::Action, void> action_up_handler;

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

};