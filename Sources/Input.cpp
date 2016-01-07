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

using namespace cross;

Input::Input(){
	for(int i = 0; i < Key::MAX_KEY_NUM; i++){
		pressed_keys[i] = false;
	}
	KeyPressed += MakeDelegate(this, &Input::KeyPressedHandle);
	KeyReleased += MakeDelegate(this, &Input::KeyReleasedHandle);
}

bool Input::IsPressed(Key key){
	return pressed_keys[key];
}

void Input::KeyPressedHandle(Key key){
	pressed_keys[key] = true;
}

void Input::KeyReleasedHandle(Key key){
	pressed_keys[key] = false;
}