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
#include "Camera3D.h"
#include "Launcher.h"
#include "Input.h"
#include "Misc.h"

using namespace cross;

Camera3D::Camera3D() : 
	liner_speed(1.f),
	angular_speed(45.f),
	yaw(0.f),
	pitch(0.f)
{
	projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
}

void Camera3D::Update(float sec){
	bool eulerAngles = false;
	if(input->IsPressed(Key::LEFT)){
		yaw -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::RIGHT)){
		yaw += angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::UP)){
		pitch -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::DOWN)){
		pitch += angular_speed * sec;
		eulerAngles = true;
	}
	if(eulerAngles){
		if(pitch > 89.0f)
			pitch = 89.0f;
		if(pitch < -89.0f)
			pitch = -89.0f;

		float cosPitch = cos(pitch / 180.f * PI);
		float cosYaw = cos(yaw / 180.f * PI);
		float sinPitch = sin(pitch / 180.f * PI);
		float sinYaw = sin(yaw / 180.f * PI);

		Vector3D direction;
		direction.z = cosPitch * cosYaw;
		direction.y = sinPitch;
		direction.x = cosPitch * sinYaw;

		SetDirection(direction);
	}


	if(input->IsPressed(Key::W)){
		SetPosition(GetPosition() + GetDirection() * liner_speed * sec);
	}
	if(input->IsPressed(Key::S)){
		SetPosition(GetPosition() - GetDirection() * liner_speed * sec);
	}
	if(input->IsPressed(Key::A)){
		SetPosition(GetPosition() + GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}
	if(input->IsPressed(Key::D)){
		SetPosition(GetPosition() - GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}
	if(input->IsPressed(Key::SHIFT)){
		SetPosition(GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}
	if(input->IsPressed(Key::CONTROL)){
		SetPosition(GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}
}