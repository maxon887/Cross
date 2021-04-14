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
#include "Transform.h"
#include "Material.h"

#include "ThirdParty/ImGui/imgui.h"

void DemoScene::Start() {
	FreeCameraScene::Start();
	if(!camera) {
		CreateDefaultCamera();
	}

	input->ActionDown.Connect(this, &DemoScene::OnActionDown);
	input->ActionMove.Connect(this, &DemoScene::OnActionMove);
	input->ActionUp.Connect(this, &DemoScene::OnActionUp);
	input->KeyPressed.Connect(this, &DemoScene::OnKeyPressed);
	input->KeyReleased.Connect(this, &DemoScene::OnKeyReleased);
	input->Scroll.Connect(this, &DemoScene::MouseWheelRoll);
	os->OrientationChanged.Connect(this, &DemoScene::OnOrientationChanged);

	LookAtTarget(Vector3D::Zero);
	LookAtCamera(true);
	
	if(os->GetDeviceOrientation() == System::Orientation::PORTRAIT) {
		OnOrientationChanged(System::Orientation::PORTRAIT);
	}

}

void DemoScene::Stop() {
	delete arrow_mat;
	if(arrow && arrow->GetParent() == nullptr) {
		delete arrow;
	}

	os->OrientationChanged.Disconnect(this, &DemoScene::OnOrientationChanged);
	input->Scroll.Disconnect(this, &DemoScene::MouseWheelRoll);
	input->KeyReleased.Disconnect(this, &DemoScene::OnKeyReleased);
	input->KeyPressed.Disconnect(this, &DemoScene::OnKeyPressed);
	input->ActionDown.Disconnect(this, &DemoScene::OnActionDown);
	input->ActionMove.Disconnect(this, &DemoScene::OnActionMove);
	input->ActionUp.Disconnect(this, &DemoScene::OnActionUp);
	FreeCameraScene::Stop();
}

void DemoScene::Update(float sec) {
	ImGuiIO& io = ImGui::GetIO();
	if(!io.WantCaptureKeyboard) {
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
		if(input->IsPressed(Key::E)) {
			FreeCameraScene::MoveUp(camera_speed * sec);
		}
		if(input->IsPressed(Key::Q)) {
			FreeCameraScene::MoveUp(-camera_speed * sec);
		}
	}

	FreeCameraScene::Update(sec);

	if(!draw_vector && arrow && arrow->GetParent()) {
		arrow->GetParent()->RemoveChild(arrow);
	}
	draw_vector = false;
}

void DemoScene::Save(const String& file) {
	if(arrow && arrow->GetParent()) {
		arrow->GetParent()->RemoveChild(arrow);
	}
	FreeCameraScene::Save(file);
}

void DemoScene::ApplyMaterial(Entity* entity, Material* mat, bool depthTest) {
	if(entity->GetComponent<Mesh>()) {
		entity->GetComponent<Mesh>()->SetMaterial(mat);
		entity->GetComponent<Mesh>()->EnableDepthTest(depthTest);
	}
	for(Entity* child : entity->GetChildren()) {
		ApplyMaterial(child, mat, depthTest);
	}
}

void DemoScene::DrawVector(const Vector3D& vec, const Vector3D& pos /* = zero */) {
	if(!arrow) {
		arrow = GetModel(ArrowModelFile)->GetHierarchy();
		arrow_mat = GetDefaultMaterial()->Clone();
		arrow_mat->SetPropertyValue("Color", Color::Blue);
		ApplyMaterial(arrow, arrow_mat, false);
	}
	if(!arrow->GetParent()) {
		AddEntity(arrow);
	}
	arrow->GetComponent<Transform>()->SetPosition(pos);
	arrow->GetComponent<Transform>()->SetDirection(vec);
	arrow->GetComponent<Transform>()->SetScale(vec.Length());
	draw_vector = true;
}

void DemoScene::OnActionDown(Input::Action action) {
	if(handled_action == -1 && !ImGui::IsMouseHoveringAnyWindow()) {
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void DemoScene::OnActionMove(Input::Action action) {
	Vector2D delta = touch_position - action.pos;
	touch_position = action.pos;

	if(handled_action == 0) {
		LookAtCamera(true);
		FreeCameraScene::LookRight(delta.x / 10.f);
		FreeCameraScene::LookUp(delta.y / 10.f);
	}
	if(handled_action == 1) {
		LookAtCamera(false);
		FreeCameraScene::LookRight(delta.x / 10.f);
		FreeCameraScene::LookUp(delta.y / 10.f);
	}
	if(handled_action == 2) {
		delta /= 200.f;
		MoveRight(delta.x);
		MoveUp(delta.y);
	}
}

void DemoScene::OnActionUp(Input::Action action) {
	if(handled_action == action.id) {
		handled_action = -1;
	}
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
		MoveCloser(0.1f * delta);
	}
}
