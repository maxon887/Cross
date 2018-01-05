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
#include "SceneView.h"
#include "CrossEditor.h"
#include "System.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"
#include "Transform.h"

SceneView::SceneView() :
	FreeCameraScene("")
{ }

SceneView::SceneView(const string& filename) :
	FreeCameraScene(filename)
{ }

void SceneView::Start() {
	FreeCameraScene::Start();
	FreeCameraScene::LookAtCamera(Vector3D::Zero);
	SetAmbientColor(Color::White);
	selection_shader = GetShader("Engine/Shaders/Simple.sha");
	selection_shader->Compile();
	selection_material = new Material(selection_shader);
	selection_material->SetPropertyValue("Color", Color("0011FFFF"));

	input->ActionDown.Connect(this, &SceneView::OnActionDown);
	input->ActionMove.Connect(this, &SceneView::OnActionMove);

	editor->GetSceneExplorer()->EntityGrabFocus.Connect(this, &SceneView::OnEntityGrabFocus);
	editor->GetSceneExplorer()->EntitySelected.Connect(this, &SceneView::OnEntitySelected);
}

void SceneView::Stop() {
	editor->GetSceneExplorer()->EntityGrabFocus.Disconnect(this, &SceneView::OnEntityGrabFocus);
	editor->GetSceneExplorer()->EntitySelected.Disconnect(this, &SceneView::OnEntitySelected);

	input->ActionDown.Disconnect(this, &SceneView::OnActionDown);
	input->ActionMove.Disconnect(this, &SceneView::OnActionMove);
	FreeCameraScene::Stop();
}

void SceneView::Update(float sec) {
	FreeCameraScene::Update(sec);
	if(selected_entity){
		Draw(selected_entity);
	}
}

void SceneView::OnActionDown(Input::Action a) {
	pos = a.pos;
}

void SceneView::OnActionMove(Input::Action a) {
	Vector2D delta = pos - a.pos;
	pos = a.pos;
	if(a.id == 0) {
		FreeCameraScene::LookAtCamera(true);
		FreeCameraScene::LookRight(delta.x / 1000.f);
		FreeCameraScene::LookUp(delta.y / 1000.f);
	}
	if(a.id == 1) {
		FreeCameraScene::LookAtCamera(false);
		FreeCameraScene::MoveForward(delta.y / 1000.f);
	}
	if(a.id == 2){
		FreeCameraScene::LookAtCamera(false);
		FreeCameraScene::MoveCameraUp(delta.y / 1500.f);
		FreeCameraScene::MoveRight(delta.x / 1500.f);
	}
}

void SceneView::OnEntitySelected(Entity* e) {
	if(selected_entity) {
		EnableMesh(selected_entity, true);
	}
	if(e) {
		EnableMesh(e, false);
	}
	selected_entity = e;
}

void SceneView::OnEntityGrabFocus(Entity* e) {
	LookAtCamera(e->GetTransform()->GetPosition());
	GetCamera()->GetTransform()->LookAt(e->GetTransform()->GetPosition());
}

void SceneView::Draw(Entity* e) {
	Mesh* mesh = e->GetComponent<Mesh>();
	Transform* transform = e->GetComponent<Transform>();
	if(mesh && transform){
		Vector3D defaultScale = transform->GetScale();
		mesh->Draw(mesh->GetMaterial(), Mesh::StencilBehaviour::WRITE);
		transform->SetScale(defaultScale * 1.1f);
		mesh->Draw(selection_material, Mesh::StencilBehaviour::READ);
		transform->SetScale(defaultScale);
	}
	for(Entity* child : e->GetChildren()){
		Draw(child);
	}
}

void SceneView::EnableMesh(Entity *e, bool value) {
	Mesh* mesh = e->GetComponent<Mesh>();
	if(mesh) {
		mesh->Enable(value);
	}
	for(Entity* child : e->GetChildren()) {
		EnableMesh(child, value);
	}
}