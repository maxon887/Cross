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
#include "DemoScene.h"
#include "Demo.h"
#include "Camera.h"
#include "Input.h"

#include "Libs/ImGui/imgui.h"

void DemoScene::Start(){
	FreeCameraScene::Start();

	action_down_del = input->ActionDown.Connect(this, &DemoScene::ActionDownHandle);
	action_move_del = input->ActionMove.Connect(this, &DemoScene::ActionMoveHandle);
	action_up_del = input->ActionUp.Connect(this, &DemoScene::ActionUpHandle);

	OnEyeClick();
}

void DemoScene::Stop(){
	input->ActionDown.Disconnect(action_down_del);
	input->ActionMove.Disconnect(action_move_del);
	input->ActionUp.Disconnect(action_up_del);
	FreeCameraScene::Stop();
}

void DemoScene::Update(float sec){
	FreeCameraScene::Update(sec);
}

void DemoScene::ActionDown(Input::Action action) {
	if(handled_action == -1) {
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void DemoScene::ActionMove(Input::Action action) {
	if(handled_action == action.id) {
		Vector2D deltaPosition = touch_position - action.pos;
		touch_position = action.pos;
		Quaternion rotateU = Quaternion(Vector3D::Up, deltaPosition.x / 10.f);
		Quaternion rotateR = Quaternion(camera->GetRight(), -deltaPosition.y / 10.f);
		camera->SetRotate(rotateU * rotateR * Quaternion(camera->GetRotation()));
		if(look_at) {				//free camera
			camera->SetPosition(target + camera->GetDirection() * orbit_distance * (-1));
		} else {
			target = camera->GetPosition() + camera->GetDirection() * orbit_distance;
		}
	}
}

void DemoScene::ActionUp(Input::Action action) {
	if(handled_action == action.id) {
		handled_action = -1;
	}
}

void DemoScene::ActionDownHandle(Input::Action action) {
	if(!ImGui::IsMouseHoveringAnyWindow()) {
		ActionDown(action);
	}
}

void DemoScene::ActionMoveHandle(Input::Action action) {
	if(!ImGui::IsMouseHoveringAnyWindow()) {
		ActionMove(action);
	}
}

void DemoScene::ActionUpHandle(Input::Action action) {
	if(!ImGui::IsMouseHoveringAnyWindow()) {
		ActionUp(action);
	}
}

void DemoScene::OnEyeClick(){
	LookAtCamera(true);
}
