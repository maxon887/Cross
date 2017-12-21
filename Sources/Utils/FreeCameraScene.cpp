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
#include "Camera.h"
#include "System.h"
#include "Input.h"
#include "Transform.h"

using namespace cross;

FreeCameraScene::FreeCameraScene(const string& filename) :
	Scene(filename)
{ }

void FreeCameraScene::Start() {
	Scene::Start();
	input->MouseWheelRoll.Connect(this, &FreeCameraScene::MouseWheelRoll);
}

void FreeCameraScene::Stop() {
    input->MouseWheelRoll.Disconnect(this, &FreeCameraScene::MouseWheelRoll);
	Scene::Stop();
}

void FreeCameraScene::Update(float sec) {
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

	if(lerp_time > 0) {
		Vector3D pos = Lerp(camera->GetPosition(), destanation, 1.f - lerp_time);
		Quaternion rot = Lerp(camera->GetTransform()->GetRotate(), orientation, 1.f - lerp_time);
		camera->SetPosition(pos);
		camera->GetTransform()->SetRotate(rot);
		lerp_time -= sec;
	}
}
 
void FreeCameraScene::MoveForward(float sec){
	if(look_at){
		orbit_distance -= liner_speed * sec;
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetTransform()->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveBackward(float sec) {
	if(look_at){
		orbit_distance += liner_speed * sec;
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetTransform()->GetDirection() * liner_speed * sec);
	}
}

void FreeCameraScene::MoveRight(float sec) {
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, -sec * angular_speed);
		camera->GetTransform()->SetRotate(rotateU * camera->GetTransform()->GetRotate());
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() + camera->GetTransform()->GetRight() * liner_speed * sec);
		target += camera->GetTransform()->GetRight() * liner_speed * sec;
	}
}

void FreeCameraScene::MoveLeft(float sec) {
	if(look_at){
		Quaternion rotateU = Quaternion(Vector3D::Up, sec * angular_speed);
		camera->GetTransform()->SetRotate(rotateU * camera->GetTransform()->GetRotate());
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}else{
		camera->SetPosition(camera->GetPosition() - camera->GetTransform()->GetRight() * liner_speed * sec);
		target -= camera->GetTransform()->GetRight() * liner_speed * sec;
	}
}

void FreeCameraScene::MoveUp(float sec) {
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + Vector3D::Up * liner_speed * sec);
		target += Vector3D::Up * liner_speed * sec;
	}
}

void FreeCameraScene::MoveDown(float sec) {
	if(!look_at){
		camera->SetPosition(camera->GetPosition() - Vector3D::Up * liner_speed * sec);
		target -= Vector3D::Up * liner_speed * sec;
	}
}

void FreeCameraScene::MoveCameraUp(float sec) {
	if(!look_at){
		camera->SetPosition(camera->GetPosition() + camera->GetTransform()->GetUp() * liner_speed * sec);
		target += camera->GetTransform()->GetUp() * liner_speed * sec;
	}
}

void FreeCameraScene::LookLeft(float sec) {
	camera->GetTransform()->SetRotate(Quaternion(Vector3D::Up, sec * angular_speed) * camera->GetTransform()->GetRotate());
	if(look_at){
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookRight(float sec) {
	camera->GetTransform()->SetRotate(Quaternion(Vector3D::Up, -sec * angular_speed) * camera->GetTransform()->GetRotate());
	if(look_at){
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookDown(float sec) {
	camera->GetTransform()->SetRotate(Quaternion(camera->GetTransform()->GetRight(), sec * angular_speed) * camera->GetTransform()->GetRotate());
	if(look_at){
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookUp(float sec) {
	camera->GetTransform()->SetRotate(Quaternion(camera->GetTransform()->GetRight(), -sec * angular_speed) * camera->GetTransform()->GetRotate());
	if(look_at){
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookAtCamera(bool enabled) {
	if(enabled){
		look_at = true;
	}else{
		look_at = false;
	}
}

void FreeCameraScene::LookAtCamera(const Vector3D& target) {
	look_at = true;
	this->target = target;
	lerp_time = 1;
	Vector3D camObjVec = target - camera->GetPosition();
	Vector3D offset = camObjVec - camObjVec.GetNormalized() * 3.f;
	destanation = camera->GetPosition() + offset;

	Transform trans;
	trans.SetPosition(destanation);
	trans.LookAt(target);
	orientation = trans.GetRotate();
}

bool FreeCameraScene::IsLookAtCamera() const {
	return look_at;
}

void FreeCameraScene::MouseWheelRoll(float delta) {
	MoveForward(0.1f * delta / 120.f);
}