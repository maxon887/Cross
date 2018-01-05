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
#include "Input.h"
#include "Camera.h"
#include "System.h"
#include "Game.h"
#include "Scene.h"

using namespace cross;

Input::Input(){
	KeyPressed.Connect(this, &Input::KeyPressedHandle);
	KeyReleased.Connect(this, &Input::KeyReleasedHandle);
	TargetActionDown.Connect(this, &Input::TargetActionDonwHandle);
	TargetActionMove.Connect(this, &Input::TargetActionMoveHandle);
	TargetActionUp.Connect(this, &Input::TargetActionUpHandle);
}

void Input::TouchEnabled(bool enabled){
	touch_enabled = enabled;
}

Vector2D Input::TargetToWordConvert(float x, float y) const {
	Vector2D result;
	result.x = x;
	result.y = system->GetWindowHeight() - y;
	return result;
}

bool Input::IsPressed(Key key) const{
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
			ActionDown(action);
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
}

void Input::KeyPressedHandle(Key key){
	pressed_keys[(U32)key] = true;
}

void Input::KeyReleasedHandle(Key key){
	pressed_keys[(U32)key] = false;
}

void Input::TargetActionDonwHandle(float x, float y, S32 actionID){
	if(touch_enabled){
		Action action;
		action.pos = Vector2D(x, y);
		action.id = actionID;
		input_mutex.lock();
		action_stack.push_back(pair<Input::Action, int>(action, 0));
		input_mutex.unlock();
	}
}

void Input::TargetActionMoveHandle(float x, float y, S32 actionID){
	if(touch_enabled){
		Action action;
		action.pos = Vector2D(x, y);
		action.id = actionID;
		input_mutex.lock();
		action_stack.push_back(pair<Input::Action, int>(action, 1));
		input_mutex.unlock();
	}
}

void Input::TargetActionUpHandle(float x, float y, S32 actionID){
	if(touch_enabled){
		Action action;
		action.pos = Vector2D(x, y);
		action.id = actionID;
		input_mutex.lock();
		action_stack.push_back(pair<Input::Action, int>(action, 2));
		input_mutex.unlock();
	}
}
