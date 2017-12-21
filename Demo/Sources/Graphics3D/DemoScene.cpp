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

#include "ThirdParty/ImGui/imgui.h"

void DemoScene::Start() {
	FreeCameraScene::Start();
	camera->SetPosition(Vector3D(0.f, 0.f, -3.0f));
	input->ActionDown.Connect(this, &DemoScene::ActionDownHandle);
	input->ActionMove.Connect(this, &DemoScene::ActionMoveHandle);
	input->ActionUp.Connect(this, &DemoScene::ActionUpHandle);
	system->OrientationChanged.Connect(this, &DemoScene::OnOrientationChanged);

	OnEyeClick();
	
	if(system->GetDeviceOrientation() == System::Orientation::PORTRAIT) {
		OnOrientationChanged(System::Orientation::PORTRAIT);
	}
}

void DemoScene::Stop() {
	system->OrientationChanged.Disconnect(this, &DemoScene::OnOrientationChanged);
	input->ActionDown.Disconnect(this, &DemoScene::ActionDownHandle);
	input->ActionMove.Disconnect(this, &DemoScene::ActionMoveHandle);
	input->ActionUp.Disconnect(this, &DemoScene::ActionUpHandle);
	FreeCameraScene::Stop();
}

void DemoScene::Update(float sec) {
	while(!action_stack.empty()) {
		Input::Action action = action_stack.front().first;
		int actionState = action_stack.front().second;
		action_stack.pop_front();
		switch(actionState) {
		case 0:
			if(!ImGui::IsMouseHoveringAnyWindow()) {
				ActionDown(action);
			}
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
		camera->GetTransform()->SetRotate(rotateU * rotateR * camera->GetTransform()->GetRotate());
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

void DemoScene::OnOrientationChanged(System::Orientation o) {
	Vector3D camPos = camera->GetPosition();
	Vector3D camDir = camera->GetTransform()->GetDirection();
	if(o == System::Orientation::PORTRAIT) {
		camera->SetPosition(camPos - camDir * 2);
	} else {
		camera->SetPosition(camPos + camDir * 2);
	}
}

void DemoScene::OnEyeClick(){
	LookAtCamera(true);
}
