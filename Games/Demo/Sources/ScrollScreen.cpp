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
#include "ScrollScreen.h"
#include "Graphics2D.h"
#include "Camera2D.h"

void ScrollScreen::Start(){
	Screen::Start();
	width = Screen::GetWidth();
	height = Screen::GetHeight();
	action_down_handler = MakeDelegate(this, &ScrollScreen::ActionDownHandle);
	action_move_handler = MakeDelegate(this, &ScrollScreen::ActionMoveHandle);
	action_up_handler = MakeDelegate(this, &ScrollScreen::ActionUpHandle);
	input->ActionDown += action_down_handler;
	input->ActionMove += action_move_handler;
	input->ActionUp += action_up_handler;
}

void ScrollScreen::Stop(){
	Screen::Stop();
	input->ActionDown -= action_down_handler;
	input->ActionMove -= action_move_handler;
	input->ActionUp -= action_up_handler;
}

float ScrollScreen::GetWidth(){
	return width;
}

float ScrollScreen::GetHeight(){
	return height;
}

void ScrollScreen::SetWidth(float width){
	this->width = width;
}

void ScrollScreen::SetHeight(float height){
	this->height = height;
}

void ScrollScreen::ResetScreenSize(){
	width = Screen::GetWidth();
	height = Screen::GetHeight();
}

void ScrollScreen::ActionDownHandle(Input::Action action){
	touch = action.pos;
}

void ScrollScreen::ActionMoveHandle(Input::Action action){
	Vector2D diff = action.pos - touch;
	diff /= 2.f;
	diff.y = -diff.y;
	touch = action.pos;

	Camera2D* cam = gfx2D->GetCamera();
	Vector2D camPos = Vector2D(cam->GetPosition().x, cam->GetPosition().y);
	camPos += diff;
	float camW = cam->GetViewWidth();
	float camH = cam->GetViewHeight();

	if(camPos.x < 0) {
		camPos.x = 0;
	}
	if(camPos.y < 0) {
		camPos.y = 0;
	}
	if(camPos.x > GetWidth() - camW){
		camPos.x = GetWidth() - camW;
	}
	if(camPos.y > GetHeight() - camH){
		camPos.y = GetHeight() - camH;
	}

	cam->SetPosition(camPos);
}

void ScrollScreen::ActionUpHandle(Input::Action action){
	ActionMoveHandle(action);
}