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
	liner_speed(50.f),
	angular_speed(45.f),
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
	if(input->IsPressed(Key::LEFT)) {
		LookLeft(sec);
	}
	if(input->IsPressed(Key::RIGHT)) {
		LookRight(sec);
	}
	if(input->IsPressed(Key::UP)) {
		LookDown(sec);
	}
	if(input->IsPressed(Key::DOWN)) {
		LookUp(sec);
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
	if(input->IsPressed(Key::E)) {
		MoveUp(sec);
	}
	if(input->IsPressed(Key::Q)) {
		MoveDown(sec);
	}

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
	if(look_at){
		orbit_distance -= liner_speed * sec;
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveBackward(float sec){
	if(look_at){
		orbit_distance += liner_speed * sec;
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveRight(float sec){
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, -sec * angular_speed);
		camera->SetRotate(rotateU * Quaternion(camera->GetRotation()));
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetRight() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveLeft(float sec){
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, sec * angular_speed);
		camera->SetRotate(rotateU * Quaternion(camera->GetRotation()));
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetRight() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveUp(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + Vector3D::Up * liner_speed * sec);
	}
}

void FreeCameraScene::MoveDown(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() - Vector3D::Up * liner_speed * sec);
	}
}

void FreeCameraScene::LookLeft(float sec){
	camera->SetRotate(Quaternion(Vector3D::Up, sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookRight(float sec){
	camera->SetRotate(Quaternion(Vector3D::Up, -sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookDown(float sec){
	camera->SetRotate(Quaternion(camera->GetRight(), sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookUp(float sec){
	camera->SetRotate(Quaternion(camera->GetRight(), -sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
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
		Vector2D deltaPosition = touch_position - action.pos;
		touch_position = action.pos;
		Quaternion rotateU = Quaternion(Vector3D::Up, deltaPosition.x / 10.f);
		Quaternion rotateR = Quaternion(camera->GetRight(), -deltaPosition.y / 10.f);
		camera->SetRotate(rotateU * rotateR * Quaternion(camera->GetRotation()));
		if(look_at){				//free camera
			camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
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
}

void FreeCameraScene::OrbitCamera(float distance){
	look_at = true;
	orbit_distance = distance;
	camera->SetPosition(camera->GetDirection() * orbit_distance * (-1));
}

void FreeCameraScene::MouseWheelUp(){
	MoveForward(0.1f);
}

void FreeCameraScene::MouseWheelDown(){
	MoveBackward(0.1f);
}