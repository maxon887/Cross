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
#include "FreeCameraScene.h"
#include "Launcher.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Sprite.h"
#include "Font.h"
#include "Camera2D.h"

#include <math.h>

FreeCameraScene::FreeCameraScene() :
	liner_speed(10.f),
	angular_speed(45.f),
	orbit_speed(1.f),
	yaw(0.f),
	pitch(0.f),
	handled_action(-1),
	orbit_distance(60.f),
	look_at(true)
{ }

void FreeCameraScene::Start() {
	Scene::Start();
	SetBackground(Color(0.3f, 0.3f, 0.3f));
	gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -orbit_distance));
	GetCamera()->SetDirection(Vector3D(0.f, 0.f, 1.f));
	debug_font = gfx2D->GetDefaultFont()->Clone();
	debug_font->SetSize(25.f);

	mouse_wheel_up = MakeDelegate(this, &FreeCameraScene::MouseWheelUp);
	mouse_wheel_down = MakeDelegate(this, &FreeCameraScene::MouseWheelDown);
	input->MouseWheelUp += mouse_wheel_up;
	input->MouseWheelDown += mouse_wheel_down;
}

void FreeCameraScene::Stop(){
	input->MouseWheelUp -= mouse_wheel_up;
	input->MouseWheelDown -= mouse_wheel_down;

	delete debug_font;

	Scene::Stop();
}

void FreeCameraScene::Update(float sec){
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
		MoveForward(sec);
	}
	if(input->IsPressed(Key::S)) {
		MoveBackward(sec);
	}
	if(input->IsPressed(Key::A)) {
		MoveLeft(sec);
	}
	if(input->IsPressed(Key::D)) {
		MoveRight(sec);
	}
	if(input->IsPressed(Key::SHIFT)) {
		MoveUp(sec);
	}
	if(input->IsPressed(Key::CONTROL)) {
		MoveDown(sec);
	}
	/*
	if(look_at){
		camera->LookAt3(Vector3D(0.f, 0.f, 0.f));
		float objectDistance = camera->GetPosition().Length();
		float diff = objectDistance - orbit_distance;
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
	}*/

	//debug camera
#ifndef ANDROID
	char buffer[256];
	Vector3D camPos = camera->GetPosition();
	sprintf(buffer, "Camera Position: %f, %f, %f", camPos.x, camPos.y, camPos.z);
	gfx2D->DrawText(Vector2D(GetWidth()/2.f, 3.f), buffer, debug_font);
	Vector3D camDir = camera->GetDirection();
	sprintf(buffer, "Camera Direction: %f, %f, %f", camDir.x, camDir.y, camDir.z);
	gfx2D->DrawText(Vector2D(GetWidth()/2.f, 3.f + debug_font->GetSize()), buffer, debug_font);
#endif // !ANDROID
	Scene::Update(sec);
}

void FreeCameraScene::MoveForward(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
	}else{
		orbit_distance -= liner_speed * sec;
	}
}

void FreeCameraScene::MoveBackward(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
	}else{
		orbit_distance += liner_speed * sec;
	}
}

void FreeCameraScene::MoveLeft(float sec){
	if(look_at){
		camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}else{
		Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
		camera->SetPosition(camera->GetPosition() + offset);
	}
}

void FreeCameraScene::MoveRight(float sec){
	if(look_at){
		camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
	}else{
		Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
		camera->SetPosition(camera->GetPosition() - offset);
	}
}

void FreeCameraScene::MoveUp(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}else{

	}
}

void FreeCameraScene::MoveDown(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
	}else{

	}
}

void FreeCameraScene::ActionDown(Input::Action action){
	if(handled_action == -1){
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void FreeCameraScene::ActionMove(Input::Action action){
	if(handled_action == action.id){
		if(!look_at){				//free camera
			Vector2D deltaPosition = touch_position - action.pos;
			touch_position = action.pos;
			yaw -= deltaPosition.x / 10;
			pitch -= deltaPosition.y / 10;
			RecalcAngles();
		}else{						//look at camera
			Vector2D deltaPosition = touch_position - action.pos;
			touch_position = action.pos;
			
			Vector3D camZX = camera->GetPosition();
			camZX.y = 0;
			camera->SetPosition(camera->GetPosition() + camera->GetRight() * deltaPosition.x * orbit_speed *  0.002f * camZX.Length());
			camera->SetPosition(camera->GetPosition() + camera->GetUp() * deltaPosition.y * orbit_speed *  0.002f * orbit_distance);

			camera->LookAt(Vector3D(0.f, 0.f, 0.f));
			float objectDistance = camera->GetPosition().Length();
			float diff = objectDistance - orbit_distance;
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
		}
	}
}

void FreeCameraScene::ActionUp(Input::Action action){
	if(handled_action == action.id){
		handled_action = -1;
	}
}

void FreeCameraScene::FreeCamera(){
	look_at = false;
	Vector3D direction = GetCamera()->GetDirection();
	float sinP = direction.y;
	float cosP = sqrt(1.f - pow(direction.y, 2.f));
	float sinY = direction.x / cosP;
	pitch = asin(sinP) * 180.f / PI;
	yaw = asin(sinY) * 180.f / PI;
	RecalcAngles();
}

void FreeCameraScene::OrbitCamera(float distance){
	look_at = true;
	orbit_distance = distance;
}

void FreeCameraScene::RecalcAngles(){
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

	GetCamera()->SetDirection(direction);
}

void FreeCameraScene::MouseWheelUp(){
	MoveForward(1.0f);
}

void FreeCameraScene::MouseWheelDown(){
	MoveBackward(1.0f);
}