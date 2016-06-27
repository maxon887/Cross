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
#include "CCScene.h"
#include "Launcher.h"
#include "Input.h"
#include "Graphics2D.h"
#include "Graphics3D.h"
#include "Sprite.h"
#include "Font.h"
#include "Demo.h"

#include <math.h>

CCScene::CCScene() :
	liner_speed(10.f),
	angular_speed(45.f),
	orbit_speed(1.f),
	yaw(0.f),
	pitch(0.f),
	eye_btn(nullptr),
	handled_action(-1),
	orbit_distance(1.f)
{ }

void CCScene::Start() {
	Scene::Start();
	gfx2D->SetClearColor(Color(0.3f, 0.3f, 0.3f));
	GetCamera()->SetPosition(Vector3D(0.f, 0.f, -20.f));
	GetCamera()->SetDirection(Vector3D(0.f, 0.f, 1.f));
	orbit_distance = GetCamera()->GetPosition().Length();
	debug_font = gfx2D->GetDefaultFont()->Clone();
	debug_font->SetSize(25.f);

	action_down_delegate = MakeDelegate(this, &CCScene::ActionDownHandle);
	action_move_delegate = MakeDelegate(this, &CCScene::ActionMoveHandle);
	action_up_delegate = MakeDelegate(this, &CCScene::ActionUpHandle);
	input->ActionDown += action_down_delegate;
	input->ActionMove += action_move_delegate;
	input->ActionUp += action_up_delegate;

	mouse_wheel_up = MakeDelegate(this, &CCScene::MouseWheelUp);
	mouse_wheel_down = MakeDelegate(this, &CCScene::MouseWheelDown);
	input->MouseWheelUp += mouse_wheel_up;
	input->MouseWheelDown += mouse_wheel_down;

	arrow_released = demo->GetCommonSprite("ArrowUp.png");
	arrow_pressed = demo->GetCommonSprite("ArrowDown.png");

	//button
	float btnWidth = 300.f;
	up_btn = new Button(Vector2D(btnWidth, btnWidth));
	Sprite* cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(90.f);
	Sprite* clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(90.f);
	up_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(up_btn);
	
	left_btn = new Button(Vector2D(btnWidth/3.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(180.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(180.f);
	left_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(left_btn);

	down_btn = new Button(Vector2D(btnWidth, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	cloneReleased->SetRotate(-90.f);
	clonePressed = arrow_pressed->Clone();
	clonePressed->SetRotate(-90.f);
	down_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(down_btn);

	right_btn = new Button(Vector2D(btnWidth + (btnWidth / 3.f) * 2.f, btnWidth/3.f));
	cloneReleased = arrow_released->Clone();
	clonePressed = arrow_pressed->Clone();
	right_btn->SetImages(cloneReleased, clonePressed);
	gui.push_back(right_btn);

	Sprite* eye = demo->GetCommonSprite("EyeBtn.png");
	Sprite* eyePressed = demo->GetCommonSprite("EyeBtnPressed.png");
	eye_btn = new ToggleButton(eye, eyePressed);
	eye_btn->Clicked += MakeDelegate(this, &CCScene::OnEyeClick);
	eye_btn->SetLocation(Vector2D(GetWidth() - eye_btn->GetWidth()/2.f, GetHeight() - eye_btn->GetHeight()/2.f));
	eye_btn->SetState(false);
	gui.push_back(eye_btn);
}

void CCScene::Stop(){
	Scene::Stop();
	input->ActionDown -= action_down_delegate;
	input->ActionMove -= action_move_delegate;
	input->ActionUp -= action_up_delegate;
	input->MouseWheelUp -= mouse_wheel_up;
	input->MouseWheelDown -= mouse_wheel_down;

	delete debug_font;
	for(Button* btn : gui){
		delete btn;
	}
	delete arrow_released;
	delete arrow_pressed;
	gui.clear();
}

void CCScene::Update(float sec){
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
	if(eye_btn->GetState()){	//free camera
		if(input->IsPressed(Key::W) || up_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::S) || down_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection() * liner_speed * sec);
		}
		if(input->IsPressed(Key::A) || left_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() + camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::D) || right_btn->IsPressed()) {
			camera->SetPosition(camera->GetPosition() - camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * liner_speed * sec);
		}
		if(input->IsPressed(Key::SHIFT)) {
			camera->SetPosition(camera->GetPosition() + Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
		if(input->IsPressed(Key::CONTROL)) {
			camera->SetPosition(camera->GetPosition() - Vector3D(0.f, 1.f, 0.f) * liner_speed * sec);
		}
	}else{	//look at camera
		if(input->IsPressed(Key::A) || left_btn->IsPressed()) {
			Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
			camera->SetPosition(camera->GetPosition() + offset);
		}
		if(input->IsPressed(Key::D) || right_btn->IsPressed()) {
			Vector3D offset = camera->GetDirection().CrossProduct(Vector3D(0.f, 1.f, 0.f)) * orbit_speed * orbit_distance * sec;
			camera->SetPosition(camera->GetPosition() - offset);
		}
		if(input->IsPressed(Key::W)) {
			camera->SetPosition(camera->GetPosition() + camera->GetUpVector() * orbit_speed * orbit_distance * sec);
		}
		if(input->IsPressed(Key::S)) {
			camera->SetPosition(camera->GetPosition() - camera->GetUpVector() * orbit_speed * orbit_distance * sec);
		}
		if(up_btn->IsPressed()){
			orbit_distance -= liner_speed * sec;
		}
		if(down_btn->IsPressed()){
			orbit_distance += liner_speed * sec;
		}
		camera->LookAt(Vector3D(0.f, 0.f, 0.f));
		float objectDistance = camera->GetPosition().Length();
		float diff = objectDistance - orbit_distance;
		camera->SetPosition(camera->GetPosition() + camera->GetDirection() * diff);
	}
	//gui
	for(Button* btn : gui){
		btn->Update();
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
	}
}

bool CCScene::OnGuiArea(Vector2D pos){
	for(Button* btn : gui){
		if(btn->OnLocation(pos)){
			return true;
		}
	}
	return false;
}

void CCScene::RecalcAngles(){
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

void CCScene::OnEyeClick(){
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

void CCScene::ActionDownHandle(Input::Action action){
	if(!OnGuiArea(action.pos) && handled_action == -1){
		handled_action = action.id;
		touch_position = action.pos;
	}
}

void CCScene::ActionMoveHandle(Input::Action action){
	if(handled_action == action.id){
		if(eye_btn->GetState()){	//free camera
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

void CCScene::ActionUpHandle(Input::Action action){
	if(handled_action == action.id){
		handled_action = -1;
	}
}

void CCScene::MouseWheelUp(){
	orbit_distance += 2.f;
}

void CCScene::MouseWheelDown(){
	orbit_distance -= 2.f;
}