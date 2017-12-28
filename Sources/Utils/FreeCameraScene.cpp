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

	if(lerp_time > 0) {
		Vector3D pos = Lerp(camera->GetPosition(), destanation, 1.f - lerp_time);
		Quaternion rot = Lerp(camera->GetTransform()->GetRotate(), orientation, 1.f - lerp_time);
		camera->SetPosition(pos);
		camera->GetTransform()->SetRotate(rot);
		lerp_time -= sec;
	}
}
 
void FreeCameraScene::MoveForward(float distance){
	Vector3D path = camera->GetTransform()->GetDirection() * distance;
	camera->SetPosition(camera->GetPosition() + path);
	target += path;
}

void FreeCameraScene::MoveRight(float distance) {
	Vector3D path = camera->GetTransform()->GetRight() * distance;
	camera->SetPosition(camera->GetPosition() + path);
	target += path;
}

void FreeCameraScene::MoveUp(float distance) {
	Vector3D path = Vector3D::Up * distance;
	camera->SetPosition(camera->GetPosition() + path);
	target += path;
}

void FreeCameraScene::MoveCameraUp(float distance) {
	Vector3D path = camera->GetTransform()->GetUp() * distance;
	camera->SetPosition(camera->GetPosition() + path);
	target += path;
}

void FreeCameraScene::LookRight(float degree) {
	camera->GetTransform()->SetRotate(Quaternion(Vector3D::Up, degree) * camera->GetTransform()->GetRotate());
	if(look_at){
		camera->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
	}
}

void FreeCameraScene::LookUp(float degree) {
	camera->GetTransform()->SetRotate(Quaternion(camera->GetTransform()->GetRight(), -degree) * camera->GetTransform()->GetRotate());
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