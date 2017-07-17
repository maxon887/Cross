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
#include "System.h"
#include "Entity.h"

SceneView::SceneView()
{ }

void SceneView::Start() {
	FreeCameraScene::Start();
}

void SceneView::Stop(){
	FreeCameraScene::Stop();
}

void SceneView::Update(float sec){
	FreeCameraScene::Update(sec);
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

void SceneView::OnEntityGrabFocus(Entity* e){
	LookAtCamera(e->GetPosition());
	GetCamera()->LookAt(e->GetPosition());
}