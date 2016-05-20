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
#include "Camera.h"
#include "Button.h"
#include "ToggleButton.h"
#include "Input.h"

using namespace cross;

//CC short for Camera Control
class CCScreen : public Screen{
public:
	CCScreen();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);
protected:
	float liner_speed;
	float angular_speed;
	float orbit_speed;
	float pitch;
	float yaw;

	Font* debug_font;

	int handled_action;
	Vector2D touch_position;
	
	Sprite* arrow_released;
	Sprite* arrow_pressed;
	CRArray<Button*> gui;
	Button* left_btn;
	Button* right_btn;
	Button* up_btn;
	Button* down_btn;
	ToggleButton* eye_btn;
	float orbit_distance;

	bool OnGuiArea(Vector2D pos);
	void RecalcAngles();
	void OnEyeClick();

	FastDelegate1<Input::Action, void> action_down_delegate;
	FastDelegate1<Input::Action, void> action_move_delegate;
	FastDelegate1<Input::Action, void> action_up_delegate;

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	FastDelegate0<void> mouse_wheel_up;
	FastDelegate0<void> mouse_wheel_down;

	void MouseWheelUp();
	void MouseWheelDown();
};