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
#include "Camera.h"
#include "System.h"
#include "Entity.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "CameraController.h"

void DemoScene::Start() {
	Scene::Start();
	if(!camera) {
		CreateDefaultCamera();
	}
	camera_controller = CREATE CameraController();
	camera->GetEntity()->AddComponent(camera_controller);

	os->OrientationChanged.Connect(this, &DemoScene::OnOrientationChanged);
	
	if(os->GetDeviceOrientation() == System::Orientation::PORTRAIT) {
		OnOrientationChanged(System::Orientation::PORTRAIT);
	}

	service_root = CREATE Entity("ServiceRoot");
	service_root->AddComponent(CREATE Transform());
	AddEntity(service_root);
}

void DemoScene::Stop() {
	delete arrow_mat;
	if(arrow && arrow->GetParent() == nullptr) {
		delete arrow;
	}

	os->OrientationChanged.Disconnect(this, &DemoScene::OnOrientationChanged);
	Scene::Stop();
}

void DemoScene::Update(float sec) {
	//service_root should always be at the end of drawing line in order to properly handle transparent drawing
	if(root->GetChildren().back() != service_root)
	{
		root->RemoveChild(service_root);
		root->AddChild(service_root);
	}

	Scene::Update(sec);

	if(!draw_vector && arrow && arrow->GetParent()) {
		arrow->GetParent()->RemoveChild(arrow);
	}
	draw_vector = false;
}

void DemoScene::Save(const String& file) {
	service_root->GetParent()->RemoveChild(service_root);
	Scene::Save(file);
	AddEntity(service_root);
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
		service_root->AddChild(arrow);
	}
	arrow->GetComponent<Transform>()->SetPosition(pos);
	arrow->GetComponent<Transform>()->SetDirection(vec);
	arrow->GetComponent<Transform>()->SetScale(vec.Length());
	draw_vector = true;
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
