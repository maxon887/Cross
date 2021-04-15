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
#include "FreeCameraScene.h"
#include "Camera.h"
#include "System.h"
#include "Input.h"
#include "Transform.h"

using namespace cross;

void FreeCameraScene::Update(float sec) {
	Scene::Update(sec);

	if(lerp_time > 0) {
		Vector3D pos = Lerp(camera->GetPosition(), destination.GetPosition(), 1.f - lerp_time);
		Quaternion rot = Lerp(camera->GetTransform()->GetRotate(), destination.GetRotate(), 1.f - lerp_time);
		camera->SetPosition(pos);
		camera->GetTransform()->SetRotate(rot);
		lerp_time -= sec;
	} else {
		camera->SetPosition(destination.GetPosition());
		camera->GetTransform()->SetRotate(destination.GetRotate());
	}
}

void FreeCameraScene::MoveForward(float distance){
	Vector3D path = destination.GetDirection() * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void FreeCameraScene::MoveRight(float distance) {
	Vector3D path = destination.GetRight() * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void FreeCameraScene::MoveUp(float distance) {
	Vector3D path = Vector3D::Up * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void FreeCameraScene::MoveCameraUp(float distance) {
	Vector3D path = destination.GetUp() * distance;
	destination.SetPosition(destination.GetPosition() + path);
}

void FreeCameraScene::MoveCloser(float ratio) {
	MoveForward(focus_distance * ratio);
	focus_distance -= focus_distance * ratio;
}

void FreeCameraScene::LookRight(float degree) {
	destination.SetRotate(Quaternion(Vector3D::Up, degree) * destination.GetRotate());
	if(look_at) {
		Vector3D target = camera->GetPosition() + camera->GetTransform()->GetForward() * focus_distance;
		destination.SetPosition(target + destination.GetForward() * focus_distance * (-1.f));
	}
}

void FreeCameraScene::LookUp(float degree) {
	destination.SetRotate(Quaternion(destination.GetRight(), -degree) * destination.GetRotate());
	if(look_at) {
		Vector3D target = camera->GetPosition() + camera->GetTransform()->GetForward() * focus_distance;
		destination.SetPosition(target + destination.GetForward() * focus_distance * (-1.f));
	}
}

void FreeCameraScene::LookAtCamera(bool enabled) {
	look_at = enabled;
}

void FreeCameraScene::LookAtTarget(const Vector3D& target, float distance /* = 3*/) {
	look_at = true;
	lerp_time = 1.f;
	focus_distance = distance;
	Vector3D camObjVec = target - camera->GetPosition();
	Vector3D offset = camObjVec - camObjVec.GetNormalized() * focus_distance;
	destination.SetPosition(camera->GetPosition() + offset);

	destination.LookAt(target);
}

bool FreeCameraScene::IsLookAtCamera() const {
	return look_at;
}