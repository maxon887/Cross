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

using namespace cross;

class CameraControlScreen : public Screen{
public:
	CameraControlScreen();

	virtual void Start();
	virtual void Stop();
	virtual void Update(float sec);
	Camera* GetCamera();
protected:
	Camera* camera;
private:
	float liner_speed;
	float angular_speed;
	float pitch;
	float yaw;
	Vector2D touch_position;
	
	Sprite* arrow_released;
	Sprite* arrow_pressed;
	Button* left_btn;
	Button* right_btn;
	Button* up_btn;
	Button* down_btn;

	void RecalcAngles();

	FastDelegate1<Vector2D, void> action_down_delegate;
	FastDelegate1<Vector2D, void> action_move_delegate;

	void ActionDownHandle(Vector2D position);
	void ActionMoveHandle(Vector2D position);
};