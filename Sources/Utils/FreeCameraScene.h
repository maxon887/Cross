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

namespace cross{

class FreeCameraScene : public Scene{
public:
	FreeCameraScene();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);

	virtual void ActionDown(Input::Action action);
	virtual void ActionMove(Input::Action action);
	virtual void ActionUp(Input::Action action);

	void LookAtCamera(bool enbled);
	void LookAtCamera(const Vector3D& target);
	bool IsLookAtCamera();

	void MoveForward(float sec);
	void MoveBackward(float sec);
	void MoveLeft(float sec);
	void MoveRight(float sec);
	void MoveUp(float sec);
	void MoveDown(float sec);
	
	void LookRight(float sec);
	void LookLeft(float sec);
	void LookDown(float sec);
	void LookUp(float sec);

protected:
	bool look_at;

	float liner_speed;
	float angular_speed;
	float orbit_distance;

	Font* debug_font;

	S32 handled_action;
	Vector2D touch_position;
	Vector3D target;

	FastDelegate0<void> mouse_wheel_up;
	FastDelegate0<void> mouse_wheel_down;

	void MouseWheelUp();
	void MouseWheelDown();
};

}