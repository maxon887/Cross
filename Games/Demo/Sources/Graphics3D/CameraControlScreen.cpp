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
#include "CameraControlScreen.h"
#include "Launcher.h"
#include "Input.h"
#include "Utils/Misc.h"

#include <math.h>

CameraControlScreen::CameraControlScreen() :
	liner_speed(1.f),
	angular_speed(45.f),
	yaw(0.f),
	pitch(0.f)
{ }

void CameraControlScreen::Start(){
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);
	camera->SetPosition(Vector3D(0.f, 0.f, -1.f));

	input->ActionDown += MakeDelegate(this, &CameraControlScreen::ActionDownHandle);
	input->ActionMove += MakeDelegate(this, &CameraControlScreen::ActionMoveHandle);
}

void CameraControlScreen::Stop(){
	delete camera;
	input->ActionDown.RemoveDelegate(input->ActionDown.GetLastDelegate());
	input->ActionMove.RemoveDelegate(input->ActionMove.GetLastDelegate());
}

void CameraControlScreen::Update(float sec){
	bool eulerAngles = false;
	if(input->IsPressed(Key::LEFT)) {
		yaw -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::RIGHT)) {
		yaw += angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::UP)) {
		pitch -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::DOWN)) {
		pitch += angular_speed * sec;
		eulerAngles = true;
	}
	if(eulerAngles) {
		RecalcAngles();
	}


	if(input->IsPressed(Key::W)) {
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
	}
	if(input->IsPressed(Key::S)) {
		camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
	}
	if(input->IsPressed(Key::A)) {
		camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}
	if(input->IsPressed(Key::D)) {
		camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}
	if(input->IsPressed(Key::SHIFT)) {
		camera->SetPosition(camera->GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}
	if(input->IsPressed(Key::CONTROL)) {
		camera->SetPosition(camera->GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}
}

Camera* CameraControlScreen::GetCamera(){
	return camera;
}

void CameraControlScreen::RecalcAngles(){
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

	camera->SetDirection(direction);
}

void CameraControlScreen::ActionDownHandle(Vector2D position){
	touch_position = position;
}

void CameraControlScreen::ActionMoveHandle(Vector2D position){
	Vector2D deltaPosition = touch_position - position;
	touch_position = position;
	yaw += deltaPosition.x / 10;
	pitch += deltaPosition.y / 10;
	RecalcAngles();
}