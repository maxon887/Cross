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
#include "Graphics2D.h"
#include "Sprite.h"

#include <math.h>

CameraControlScreen::CameraControlScreen() :
	liner_speed(1.f),
	angular_speed(45.f),
	yaw(0.f),
	pitch(0.f),
	left_btn(nullptr),
	right_btn(nullptr),
	up_btn(nullptr),
	down_btn(nullptr)
{ }

void CameraControlScreen::Start() {
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, launcher->GetAspectRatio(), 0.1f, 100.f);
	camera = new Camera(projection);
	camera->SetPosition(Vector3D(0.f, 0.f, -1.f));

	action_down_delegate = MakeDelegate(this, &CameraControlScreen::ActionDownHandle);
	action_move_delegate = MakeDelegate(this, &CameraControlScreen::ActionMoveHandle);
	input->ActionDown += action_down_delegate;
	input->ActionMove += action_move_delegate;

	arrow_released = gfx2D->LoadImage("ArrowUp.png");
	arrow_pressed = gfx2D->LoadImage("ArrowDown.png");
	arrow_released->SetScale(0.5f);
	arrow_pressed->SetScale(0.5f);

	//button
	up_btn = new Button(Vector2D(150.f, 150.f));
	Sprite* cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(90);
	Sprite* clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(90);
	up_btn->SetImages(cloneReleased, clonePressed);
	left_btn = new Button(Vector2D(50.f, 50.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(180);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(180);
	left_btn->SetImages(cloneReleased, clonePressed);
	down_btn = new Button(Vector2D(150.f, 50.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(-90);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(-90);
	down_btn->SetImages(cloneReleased, clonePressed);
	right_btn = new Button(Vector2D(250.f, 50.f));
	cloneReleased = arrow_released->Clone();
	clonePressed = arrow_pressed->Clone();
	right_btn->SetImages(cloneReleased, clonePressed);
}

void CameraControlScreen::Stop(){
	delete camera;
	input->ActionDown -= action_down_delegate;
	input->ActionMove -= action_move_delegate;

	delete arrow_released;
	delete arrow_pressed;
	delete up_btn;
	delete down_btn;
	delete left_btn;
	delete right_btn;
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
	//gui
	up_btn->Update();
	left_btn->Update();
	right_btn->Update();
	down_btn->Update();
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