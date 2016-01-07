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
#pragma once
#include "Cross.h"
#include "Events\Event.h"

namespace cross {

enum Key{
	UNDEFINED 	= 0,
	PAUSE		= 1,
	BACK		= 2,
	OPTIONS		= 3,
	UP			= 4,
	DOWN		= 5,
	RIGHT       = 6,
	LEFT        = 7,
	ENTER       = 8,
	SPACE		= 9,
	SHIFT		= 10,
	ESCAPE		= 11,
	W			= 12,
	A			= 13,
	S			= 14,
	D			= 15,
	CONTROL		= 16,
	MAX_KEY_NUM = 17
};

/*	Class responsible for user input. 
	Handle touches, clicks and key events */
class Input{
public:
	DECLARE_EVENT(void, Vector2D) ActionDown;
	DECLARE_EVENT(void, Vector2D) ActionUp;
	DECLARE_EVENT(void, Vector2D) ActionMove;

	DECLARE_EVENT(void, Key) KeyPressed;
	DECLARE_EVENT(void, Key) KeyReleased;

	bool IsPressed(Key key);
//internal
public:
	Input();
private:
	void KeyPressedHandle(Key key);
	void KeyReleasedHandle(Key key);
	bool pressed_keys[Key::MAX_KEY_NUM];
};

}
