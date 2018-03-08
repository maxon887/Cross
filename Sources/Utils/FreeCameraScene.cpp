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

FreeCameraScene::FreeCameraScene(const String& filename)
	: Scene(filename)
{ }

void FreeCameraScene::Update(float sec) {
	Scene::Update(sec);

	if(lerp_time > 0) {
		Vector3D pos = Lerp(camera->GetPosition(), destanation.GetPosition(), 1.f - lerp_time);
		Quaternion rot = Lerp(camera->GetTransform()->GetRotate(), destanation.GetRotate(), 1.f - lerp_time);
		camera->SetPosition(pos);
		camera->GetTransform()->SetRotate(rot);
		lerp_time -= sec;
	} else {
		camera->SetPosition(destanation.GetPosition());
		camera->GetTransform()->SetRotate(destanation.GetRotate());
	}
}
 
void FreeCameraScene::MoveForward(float distance, bool transferTraget){
	Vector3D path = destanation.GetDirection() * distance;
	destanation.SetPosition(destanation.GetPosition() + path);
	if(transferTraget) {
		target += path;
	}
}

void FreeCameraScene::MoveRight(float distance) {
	Vector3D path = destanation.GetRight() * distance;
	destanation.SetPosition(destanation.GetPosition() + path);
	target += path;
}

void FreeCameraScene::MoveUp(float distance) {
	Vector3D path = Vector3D::Up * distance;
	destanation.SetPosition(destanation.GetPosition() + path);
	target += path;
}

void FreeCameraScene::MoveCameraUp(float distance) {
	Vector3D path = destanation.GetUp() * distance;
	destanation.SetPosition(destanation.GetPosition() + path);
	target += path;
}

void FreeCameraScene::LookRight(float degree) {
	destanation.SetRotate(Quaternion(Vector3D::Up, degree) * destanation.GetRotate());
	if(look_at) {
		float distance = Vector3D(target - destanation.GetPosition()).Length();
		destanation.SetPosition(target + destanation.GetForward() * distance * (-1.f));
	}
}

void FreeCameraScene::LookUp(float degree) {
	destanation.SetRotate(Quaternion(destanation.GetRight(), -degree) * destanation.GetRotate());
	if(look_at) {
		float distance = Vector3D(target - destanation.GetPosition()).Length();
		destanation.SetPosition(target + destanation.GetForward() * distance * (-1.f));
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
	lerp_time = 1.f;
	Vector3D camObjVec = target - camera->GetPosition();
	Vector3D offset = camObjVec - camObjVec.GetNormalized() * focus_distance;
	destanation.SetPosition(camera->GetPosition() + offset);

	destanation.LookAt(target);
}

bool FreeCameraScene::IsLookAtCamera() const {
	return look_at;
}