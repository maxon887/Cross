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
#include "System.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Sprite.h"
#include "Utils/Font.h"
#include "Camera2D.h"

using namespace cross;

void FreeCameraScene::Start() {
	Scene::Start();

	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -orbit_distance));
	GetCamera()->SetDirection(Vector3D(0.f, 0.f, 1.f));
	debug_font = gfx2D->GetDefaultFont()->Clone();
	debug_font->SetSize(25.f);
	wheel_roll = input->MouseWheelRoll.Connect(this, &FreeCameraScene::MouseWheelRoll);
}

void FreeCameraScene::Stop(){
    input->MouseWheelRoll.Disconnect(wheel_roll);

	delete debug_font;

	Scene::Stop();
}

void FreeCameraScene::Update(float sec){
	Scene::Update(sec);
	Vector3D tgv = target - GetCamera()->GetPosition();
	orbit_distance = tgv.Length();

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
}

void FreeCameraScene::MoveForward(float sec){
	if(look_at){
		orbit_distance -= liner_speed * sec;
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveBackward(float sec){
	if(look_at){
		orbit_distance += liner_speed * sec;
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveRight(float sec){
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, -sec * angular_speed);
		camera->SetRotate(rotateU * Quaternion(camera->GetRotation()));
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetRight() * liner_speed * sec);
		target += camera->GetRight() * liner_speed * sec;
	}
}

void FreeCameraScene::MoveLeft(float sec){
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, sec * angular_speed);
		camera->SetRotate(rotateU * Quaternion(camera->GetRotation()));
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetRight() * liner_speed * sec);
		target -= camera->GetRight() * liner_speed * sec;
	}
}

void FreeCameraScene::MoveUp(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + Vector3D::Up * liner_speed * sec);
		target += Vector3D::Up * liner_speed * sec;
	}
}

void FreeCameraScene::MoveDown(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() - Vector3D::Up * liner_speed * sec);
		target -= Vector3D::Up * liner_speed * sec;
	}
}

void FreeCameraScene::MoveCameraUp(float sec){
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + camera->GetUp() * liner_speed * sec);
		target += camera->GetUp() * liner_speed * sec;
	}
}

void FreeCameraScene::LookLeft(float sec){
	camera->SetRotate(Quaternion(Vector3D::Up, sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookRight(float sec){
	camera->SetRotate(Quaternion(Vector3D::Up, -sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookDown(float sec){
	camera->SetRotate(Quaternion(camera->GetRight(), sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookUp(float sec){
	camera->SetRotate(Quaternion(camera->GetRight(), -sec * angular_speed) * Quaternion(camera->GetRotation()));
	if(look_at){
		camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
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
			camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
		}else{
			target = camera->GetPosition() + camera->GetDirection() * orbit_distance;
		}
	}
}

void FreeCameraScene::ActionUp(Input::Action action){
	if(handled_action == action.id){
		handled_action = -1;
	}
}

void FreeCameraScene::LookAtCamera(bool enabled){
	if(enabled){
		look_at = true;
	}else{
		look_at = false;
	}
}

void FreeCameraScene::LookAtCamera(const Vector3D& target){
	look_at = true;
	this->target = target;
}

bool FreeCameraScene::IsLookAtCamera() const{
	return look_at;
}

void FreeCameraScene::MouseWheelRoll(float delta) {
	MoveForward(0.1f * delta / 120.f);
}