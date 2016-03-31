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
#include "Launcher.h"

using namespace cross;

Input::Input(){
	for(unsigned int i = 0; i < (unsigned int)Key::MAX_KEY_NUM; i++){
		pressed_keys[i] = false;
	}
	KeyPressed += MakeDelegate(this, &Input::KeyPressedHandle);
	KeyReleased += MakeDelegate(this, &Input::KeyReleasedHandle);
	TargetActionDown += MakeDelegate(this, &Input::TargetActionDonwHandle);
	TargetActionMove += MakeDelegate(this, &Input::TargetActionMoveHandle);
	TargetActionUp += MakeDelegate(this, &Input::TargetActionUpHandle);
}

Vector2D Input::TargetToWordConvert(float x, float y){
	Vector2D result;
	Camera2D* cam = gfx2D->GetCamera();
	float scaleFactor = launcher->GetTargetWidth() / cam->GetViewWidth();
	result.x = x / scaleFactor;
	result.y = cam->GetViewHeight() - y / scaleFactor;
	result.x += cam->GetPosition().x;
	result.y += cam->GetPosition().y;
	return result;
}

bool Input::IsPressed(Key key){
	return pressed_keys[(unsigned int)key];
}

void Input::KeyPressedHandle(Key key){
	pressed_keys[(unsigned int)key] = true;
}

void Input::KeyReleasedHandle(Key key){
	pressed_keys[(unsigned int)key] = false;
}

void Input::TargetActionDonwHandle(float x, float y){
	TRIGGER_EVENT(ActionDown, TargetToWordConvert(x, y));
}

void Input::TargetActionMoveHandle(float x, float y){
	TRIGGER_EVENT(ActionMove, TargetToWordConvert(x, y));
}

void Input::TargetActionUpHandle(float x, float y){
	TRIGGER_EVENT(ActionUp, TargetToWordConvert(x, y));
}