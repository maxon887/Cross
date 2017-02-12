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
#include "Input.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#include "System.h"

using namespace cross;

Input::Input(){
	memset(pressed_keys, 0, sizeof(pressed_keys));
	KeyPressed += MakeDelegate(this, &Input::KeyPressedHandle);
	KeyReleased += MakeDelegate(this, &Input::KeyReleasedHandle);
	TargetActionDown += MakeDelegate(this, &Input::TargetActionDonwHandle);
	TargetActionMove += MakeDelegate(this, &Input::TargetActionMoveHandle);
	TargetActionUp += MakeDelegate(this, &Input::TargetActionUpHandle);
}

Vector2D Input::TargetToWordConvert(float x, float y){
	Vector2D result;
	Camera2D* cam = gfx2D->GetCamera();
	float scaleFactor = system->GetWindowWidth() / cam->GetViewWidth();
	result.x = x / scaleFactor;
	result.y = cam->GetViewHeight() - y / scaleFactor;
	result.x += cam->GetPosition().x;
	result.y += cam->GetPosition().y;
	return result;
}

bool Input::IsPressed(Key key){
	return pressed_keys[(U32)key];
}

void Input::Update(){
	while(!action_stack.empty()){
		input_mutex.lock();
		Action action = action_stack.front().first;
		int actionState = action_stack.front().second;
		action_stack.pop_front();
		input_mutex.unlock();

		action.pos = TargetToWordConvert(action.pos.x, action.pos.y);
		switch(actionState)	{
		case 0:
			TRIGGER_EVENT(ActionDown, action);
			break;
		case 1:
			TRIGGER_EVENT(ActionMove, action);
			break;
		case 2:
			TRIGGER_EVENT(ActionUp, action);
			break;
		default:
			break;
		}
	}
}

void Input::KeyPressedHandle(Key key){
	pressed_keys[(U32)key] = true;
}

void Input::KeyReleasedHandle(Key key){
	pressed_keys[(U32)key] = false;
}

void Input::TargetActionDonwHandle(float x, float y, S32 actionID){
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.push_back(pair<Input::Action, int>(action, 0));
	input_mutex.unlock();
}

void Input::TargetActionMoveHandle(float x, float y, S32 actionID){
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.push_back(pair<Input::Action, int>(action, 1));
	input_mutex.unlock();
}

void Input::TargetActionUpHandle(float x, float y, S32 actionID){
	Action action;
	action.pos = Vector2D(x, y);
	action.id = actionID;
	input_mutex.lock();
	action_stack.push_back(pair<Input::Action, int>(action, 2));
	input_mutex.unlock();
}