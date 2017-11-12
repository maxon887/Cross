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
#include "System.h"
#include "Entity.h"
#include "Mesh.h"
#include "Transform.h"

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
	while(!action_stack.empty()) {
		Input::Action action = action_stack.front().first;
		int actionState = action_stack.front().second;
		action_stack.pop_front();
		if (!ImGui::IsMouseHoveringAnyWindow())	{
			switch(actionState) {
			case 0:
				ActionDown(action);
				break;
			case 1:
				ActionMove(action);
				break;
			case 2:
				ActionUp(action);
				break;
			default:
				break;
			}
		}
	}

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
		Quaternion rotateR = Quaternion(camera->GetTransform()->GetRight(), -deltaPosition.y / 10.f);
		camera->GetTransform()->SetRotate(rotateU * rotateR * Quaternion(camera->GetTransform()->GetRotation()));
		if(look_at) {				//free camera
			camera->GetTransform()->SetPosition(target + camera->GetTransform()->GetDirection() * orbit_distance * (-1));
		} else {
			target = camera->GetPosition() + camera->GetTransform()->GetDirection() * orbit_distance;
		}
	}
}

void DemoScene::ActionUp(Input::Action action) {
	if(handled_action == action.id) {
		handled_action = -1;
	}
}

void DemoScene::ApplyMaterial(Entity* entity, Material* mat) {
	if(entity->GetComponent<Mesh>()) {
		entity->GetComponent<Mesh>()->SetMaterial(mat);
	}
	for(Entity* child : entity->GetChildren()) {
		ApplyMaterial(child, mat);
	}
}

void DemoScene::ActionDownHandle(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 0));
}

void DemoScene::ActionMoveHandle(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 1));
}

void DemoScene::ActionUpHandle(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 2));
}

void DemoScene::OnEyeClick(){
	LookAtCamera(true);
}
