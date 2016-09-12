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
#include "Launcher.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Sprite.h"
#include "Font.h"
#include "Camera2D.h"
#include "Game.h"

#include "Material.h"
#include "VertexBuffer.h"
#include "Mesh.h"

#include <math.h>

SceneView::SceneView() :
	liner_speed(10.f),
	angular_speed(45.f),
	orbit_speed(1.f),
	yaw(0.f),
	pitch(0.f),
	handled_action(-1),
	orbit_distance(1.f)
{ }

void SceneView::Start() {
	Scene::Start();
	SetBackground(Color(0.3f, 0.3f, 0.3f));
	gfx2D->GetCamera()->SetPosition(Vector2D(0.f, 0.f));
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -20.f));
	GetCamera()->SetDirection(Vector3D(0.f, 0.f, 1.f));
	orbit_distance = GetCamera()->GetPosition().Length();
	debug_font = gfx2D->GetDefaultFont()->Clone();
	debug_font->SetSize(25.f);

	action_down_delegate = MakeDelegate(this, &SceneView::ActionDownHandle);
	action_move_delegate = MakeDelegate(this, &SceneView::ActionMoveHandle);
	action_up_delegate = MakeDelegate(this, &SceneView::ActionUpHandle);
	input->ActionDown += action_down_delegate;
	input->ActionMove += action_move_delegate;
	input->ActionUp += action_up_delegate;

	mouse_wheel_up = MakeDelegate(this, &SceneView::MouseWheelUp);
	mouse_wheel_down = MakeDelegate(this, &SceneView::MouseWheelDown);
	input->MouseWheelUp += mouse_wheel_up;
	input->MouseWheelDown += mouse_wheel_down;

	//button
	float btnWidth = 300.f;

	InitializeTriangle();
}

void SceneView::Stop(){
	input->ActionDown -= action_down_delegate;
	input->ActionMove -= action_move_delegate;
	input->ActionUp -= action_up_delegate;
	input->MouseWheelUp -= mouse_wheel_up;
	input->MouseWheelDown -= mouse_wheel_down;

	delete debug_font;
	//triangle
	delete triangle_material;
	delete triangle;
	Scene::Stop();
}

void SceneView::Update(float sec){
	bool eulerAngles = false;
	if(input->IsPressed(Key::LEFT)) {
		yaw -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::RIGHT)) {
		yaw += angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::UP)) {
		pitch -= angular_speed * sec;
		eulerAngles = true;
	}
	if(input->IsPressed(Key::DOWN)) {
		pitch += angular_speed * sec;
		eulerAngles = true;
	}
	if(eulerAngles) {
		RecalcAngles();
	}
	if(false){	//free camera
		if(input->IsPressed(Key::W)) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::S)) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::A)) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::D)) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::SHIFT)) {
			camera->SetPosition(camera->GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
		if(input->IsPressed(Key::CONTROL)) {
			camera->SetPosition(camera->GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
	}else{	//look at camera
		if(input->IsPressed(Key::A)) {
			Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
			camera->SetPosition(camera->GetPosition() + offset);
		}
		if(input->IsPressed(Key::D)) {
			Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
			camera->SetPosition(camera->GetPosition() - offset);
		}
		if(input->IsPressed(Key::W)) {
			camera->SetPosition(camera->GetPosition() + camera->GetUpVector() * orbit_speed * orbit_distance * sec);
		}
		if(input->IsPressed(Key::S)) {
			camera->SetPosition(camera->GetPosition() - camera->GetUpVector() * orbit_speed * orbit_distance * sec);
		}
		camera->LookAt(Vector3D(0.f, 0.f, 0.f));
		float objectDistance = camera->GetPosition().Length();
		float diff = objectDistance - orbit_distance;
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
	}
	//debug camera
#ifndef ANDROID
	char buffer[256];
	Vector3D camPos = camera->GetPosition();
	sprintf(buffer, "Camera Position: %f, %f, %f", camPos.x, camPos.y, camPos.z);
	gfx2D->DrawText(Vector2D(GetWidth()/2.f, 3.f), buffer, debug_font);
	Vector3D camDir = camera->GetDirection();
	sprintf(buffer, "Camera Direction: %f, %f, %f", camDir.x, camDir.y, camDir.z);
	gfx2D->DrawText(Vector2D(GetWidth()/2.f, 3.f + debug_font->GetSize()), buffer, debug_font);
#endif // !ANDROID
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
		return;
	}
	triangle->Draw();
	triangle->SetRotateY(game->GetRunTime() * sec);
	launcher->LogIt("Run Time - %f", game->GetRunTime() * sec);

	Scene::Update(sec);
}

void SceneView::SetOrbitDistance(float orbitDistance){
	orbit_distance = orbitDistance;
}

void SceneView::InitializeTriangle(){
	Shader* shader = gfxGL->GetShader(DefaultShader::SIMPLE);
	triangle_material = new Material(shader);
	triangle_material->SetDiffuseColor(Color::Red);

	VertexBuffer* vertexBuffer = new VertexBuffer();
	
	Vector3D verticesData[3];
	verticesData[0].x = 0.0f;  verticesData[0].y = 5.f;  verticesData[0].z = 0.0f;
	verticesData[1].x = -5.f;  verticesData[1].y = -5.f;  verticesData[1].z = 0.0f;
	verticesData[2].x = 5.f;  verticesData[2].y = -5.f;  verticesData[2].z = 0.0f;

	vertexBuffer->PushData((CRByte*)&verticesData[0], 3 * sizeof(Vector3D));

	CRArray<unsigned int> indices = { 0, 1, 2 };

	triangle = new Mesh(vertexBuffer, indices, indices.size());
	triangle->SetMaterial(triangle_material);
}

bool SceneView::OnGuiArea(Vector2D pos){
	return false;
}

void SceneView::RecalcAngles(){
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	float cosPitch = cos(pitch / 180.f * PI);
	float cosYaw = cos(yaw / 180.f * PI);
	float sinPitch = sin(pitch / 180.f * PI);
	float sinYaw = sin(yaw / 180.f * PI);

	Vector3D direction;
	direction.z = cosPitch * cosYaw;
	direction.y = sinPitch;
	direction.x = cosPitch * sinYaw;

	GetCamera()->SetDirection(direction);
}

void SceneView::OnEyeClick(){
	//orbit
	orbit_distance = GetCamera()->GetPosition().Length();
	//free
	Vector3D direction = GetCamera()->GetDirection();
	float sinP = direction.y;
	float cosP = sqrt(1.f - pow(direction.y, 2.f));
	float sinY = direction.x / cosP;
	pitch = asin(sinP) * 180.f / PI;
	yaw = asin(sinY) * 180.f / PI;
	RecalcAngles();
}

void SceneView::ActionDownHandle(Input::Action action){
	if(!OnGuiArea(action.pos) && handled_action == -1){
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void SceneView::ActionMoveHandle(Input::Action action){
	if(handled_action == action.id){
		if(false){	//free camera
			Vector2D deltaPosition = touch_position - action.pos;
			touch_position = action.pos;
			yaw += deltaPosition.x / 10;
			pitch += deltaPosition.y / 10;
			RecalcAngles();
		}else{						//look at camera
			Vector2D deltaPosition = touch_position - action.pos;
			touch_position = action.pos;

			Vector3D horizontal = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f));
			Vector3D vertical = camera->GetUpVector();
			camera->SetPosition(camera->GetPosition() + horizontal * deltaPosition.x * orbit_speed *  0.002f * orbit_distance);
			camera->SetPosition(camera->GetPosition() + vertical * deltaPosition.y * orbit_speed * 0.002f * orbit_distance);

			camera->LookAt(Vector3D(0.f, 0.f, 0.f));
			float objectDistance = camera->GetPosition().Length();
			float diff = objectDistance - orbit_distance;
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
		}
	}
}

void SceneView::ActionUpHandle(Input::Action action){
	if(handled_action == action.id){
		handled_action = -1;
	}
}

void SceneView::MouseWheelUp(){
	orbit_distance += 2.f;
}

void SceneView::MouseWheelDown(){
	orbit_distance -= 2.f;
}