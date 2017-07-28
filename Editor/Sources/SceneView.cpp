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
#include "GraphicsGL.h"
#include "System.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Camera.h"

SceneView::SceneView()
{ }

void SceneView::Start() {
	FreeCameraScene::Start();
	SetAmbientColor(Color::White);
	selection_shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	selection_shader->Compile();
	selection_material = new Material(selection_shader);
}

void SceneView::Stop(){
	FreeCameraScene::Stop();
}

void SceneView::Update(float sec){
	FreeCameraScene::Update(sec);
	if(selected_entity){
		Draw(selected_entity);
	}
}

void SceneView::ActionDown(Input::Action a){
	FreeCameraScene::ActionDown(a);
	if(a.id == 2){
		pos = a.pos;
	}
}

void SceneView::ActionMove(Input::Action a){
	if(a.id == 0 && input->IsPressed(Key::ALT)){
		LookAtCamera(true);
		FreeCameraScene::ActionMove(a);
	}
	if(a.id == 1) {
		LookAtCamera(false);
		FreeCameraScene::ActionMove(a);
	}
	if(a.id == 2){
		LookAtCamera(false);
		Vector2D delta = pos - a.pos;
		pos = a.pos;
		MoveCameraUp(delta.y / 1500.f);
		MoveRight(delta.x / 1500.f);
	}
}

void SceneView::OnEntitySelected(Entity* e){
	if(selected_entity) {
		EnableMesh(selected_entity, true);
	}
	if(e){
		EnableMesh(e, false);
		selected_entity = e;
	}
}

void SceneView::OnEntityGrabFocus(Entity* e){
	LookAtCamera(e->GetPosition());
	GetCamera()->LookAt(e->GetPosition());
}

void SceneView::Draw(Entity* e){
	Mesh* mesh = e->GetComponent<Mesh>();
	if(mesh){
		mesh->Draw(selection_material);
	}
	for(Entity* child : e->GetChildren()){
		Draw(child);
	}
}

void SceneView::EnableMesh(Entity *e, bool value){
	Mesh* mesh = e->GetComponent<Mesh>();
	if(mesh) {
		mesh->Enable(value);
	}
	for(Entity* child : e->GetChildren()) {
		EnableMesh(child, value);
	}
}