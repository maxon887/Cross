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
#include "Scene.h"
#include "Input.h"

using namespace cross;

class FreeCameraScene : public Scene{
public:
	FreeCameraScene();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

	virtual void ActionDown(Input::Action action);
	virtual void ActionMove(Input::Action action);
	virtual void ActionUp(Input::Action action);

	void FreeCamera();
	void OrbitCamera(float distance);

	void MoveForward(float sec);
	void MoveBackward(float sec);
	void MoveLeft(float sec);
	void MoveRight(float sec);
	void MoveUp(float sec);
	void MoveDown(float sec);
private:
	bool look_at;

	float liner_speed;
	float angular_speed;
	float orbit_speed;
	float pitch;
	float yaw;

	Font* debug_font;

	S32 handled_action;
	Vector2D touch_position;

	float orbit_distance;

	void RecalcAngles();

	FastDelegate0<void> mouse_wheel_up;
	FastDelegate0<void> mouse_wheel_down;

	void MouseWheelUp();
	void MouseWheelDown();
};