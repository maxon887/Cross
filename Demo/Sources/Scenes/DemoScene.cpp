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
#include "DemoScene.h"
#include "Demo.h"
#include "Camera.h"
#include "Input.h"
#include "System.h"
#include "Entity.h"
#include "Mesh.h"

#include "ThirdParty/ImGui/imgui.h"

void DemoScene::Start() {
	FreeCameraScene::Start();
	if(!camera) {
		CreateDefaultCamera();
	}
	LookAtCamera(Vector3D::Zero);
	input->ActionDown.Connect(this, &DemoScene::OnActionDown);
	input->ActionMove.Connect(this, &DemoScene::OnActionMove);
	input->ActionUp.Connect(this, &DemoScene::OnActionUp);
	input->KeyPressed.Connect(this, &DemoScene::OnKeyPressed);
	input->KeyReleased.Connect(this, &DemoScene::OnKeyReleased);
	input->Scroll.Connect(this, &DemoScene::MouseWheelRoll);
	system->OrientationChanged.Connect(this, &DemoScene::OnOrientationChanged);

	LookAtCamera(true);
	
	if(system->GetDeviceOrientation() == System::Orientation::PORTRAIT) {
		OnOrientationChanged(System::Orientation::PORTRAIT);
	}
}

void DemoScene::Stop() {
	system->OrientationChanged.Disconnect(this, &DemoScene::OnOrientationChanged);
	input->Scroll.Disconnect(this, &DemoScene::MouseWheelRoll);
	input->KeyReleased.Disconnect(this, &DemoScene::OnKeyReleased);
	input->KeyPressed.Disconnect(this, &DemoScene::OnKeyPressed);
	input->ActionDown.Disconnect(this, &DemoScene::OnActionDown);
	input->ActionMove.Disconnect(this, &DemoScene::OnActionMove);
	input->ActionUp.Disconnect(this, &DemoScene::OnActionUp);
	FreeCameraScene::Stop();
}

void DemoScene::Update(float sec) {

	if(input->IsPressed(Key::W)) {
		FreeCameraScene::MoveForward(camera_speed * sec);
	}
	if(input->IsPressed(Key::S)) {
		FreeCameraScene::MoveForward(-camera_speed * sec);
	}
	if(input->IsPressed(Key::D)) {
		FreeCameraScene::MoveRight(camera_speed * sec);
	}
	if(input->IsPressed(Key::A)) {
		FreeCameraScene::MoveRight(-camera_speed * sec);
	}
	if(input->IsPressed(Key::SHIFT)) {
		FreeCameraScene::MoveUp(camera_speed * sec);
	}
	if(input->IsPressed(Key::CONTROL)) {
		FreeCameraScene::MoveUp(-camera_speed * sec);
	}

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

	transform_gizmo.Update(sec);
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
		FreeCameraScene::LookRight(deltaPosition.x / 10.f);
		FreeCameraScene::LookUp(deltaPosition.y / 10.f);
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

void DemoScene::OnActionDown(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 0));
}

void DemoScene::OnActionMove(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 1));
}

void DemoScene::OnActionUp(Input::Action action) {
	action_stack.push_back(pair<Input::Action, int>(action, 2));
}

void DemoScene::OnKeyPressed(Key key) {
	if(key == Key::ALT) {
		FreeCameraScene::LookAtCamera(false);
	}
}

void DemoScene::OnKeyReleased(Key key) {
	if(key == Key::ALT) {
		FreeCameraScene::LookAtCamera(true);
	}
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

void DemoScene::MouseWheelRoll(float delta) {
	if(!ImGui::IsMouseHoveringAnyWindow()) {
		MoveForward(0.1f * delta, false);
	}
}
