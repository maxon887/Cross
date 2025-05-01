/*	Copyright © 2018 Maksim Lukyanov

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
#include "Component.h"
#include "Input.h"
#include "Transform.h"

using namespace cross;

class CameraController : public Component {
public:
	CameraController();

	void Initialize(Scene* scene) override;
	void Remove() override;
	void Update(float sec) override;
	
	void LookAtTarget(const Vector3D& target, float distance = 3);
	bool IsLookAtCamera() const;

	void MoveForward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);
	void MoveCloser(float ratio);	//how close you will be compared with current distance

	void LookRight(float degree);
	void LookUp(float degree);

protected:
	enum Mode {
		ORBIT,
		FREE,
		PAD
	};
		
	Property<float> camera_speed = Property<float>(this, "CameraSpeed", 5.f);
	
	bool camera_active			= false;
	Mode mode		 			= Mode::ORBIT;

	float focus_distance		= 3.f;
	float lerp_time				= 0.f;
	Transform destination		= Transform();
	
	Vector2D touch_position = Vector2D();
	
	void OnActionDown(Input::Action action);
	void OnActionMove(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnKeyPressed(Key key);
	void OnKeyReleased(Key key);
	void MouseWheelRoll(float delta);
};