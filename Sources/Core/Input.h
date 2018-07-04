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
#pragma once
#include "Cross.h"
#include "Event.h"

#include <mutex>

namespace cross {

#define MAX_ACTIONS 20

enum class Key {
	BACK		= 0x08,
	TAB			= 0x09,
	CLEAR		= 0x0C,
	RETURN		= 0x0D,
	SHIFT		= 0x10,
	CONTROL		= 0x11,
	ALT			= 0x12,
	PAUSE		= 0x13,
	ESCAPE		= 0x1B,
	SPACE		= 0x20,
	PAGE_UP		= 0x21,
	PAGE_DOWN	= 0x22,
	END			= 0x23,
	HOME		= 0x24,
	LEFT		= 0x25,
	UP			= 0x26,
	RIGHT		= 0x27,
	DOWN		= 0x28,
	INSERT		= 0x2D,
	DEL			= 0x2E,
	HELP		= 0x2F,
	NUM_0		= 0x30,
	NUM_1		= 0x31,
	NUM_2		= 0x32,
	NUM_3		= 0x33,
	NUM_4		= 0x34,
	NUM_5		= 0x35,
	NUM_6		= 0x36,
	NUM_7		= 0x37,
	NUM_8		= 0x38,
	NUM_9		= 0x39,

	A			= 0x41,
	B			= 0x42,
	C			= 0x43,
	D			= 0x44,
	E			= 0x45,
	F			= 0x46,
	G			= 0x47,
	H			= 0x48,
	I			= 0x49,
	J			= 0x4A,
	K			= 0x4B,
	L			= 0x4C,
	M			= 0x4D,
	N			= 0x4E,
	O			= 0x4F,
	P			= 0x50,
	Q			= 0x51,
	R			= 0x52,
	S			= 0x53,
	T			= 0x54,
	U			= 0x55,
	V			= 0x56,
	W			= 0x57,
	X			= 0x58,
	Y			= 0x59,
	Z			= 0x5A,

	NUMPAD_0	= 0x60,
	NUMPAD_1	= 0x61,
	NUMPAD_2	= 0x62,
	NUMPAD_3	= 0x63,
	NUMPAD_4	= 0x64,
	NUMPAD_5	= 0x65,
	NUMPAD_6	= 0x66,
	NUMPAD_7	= 0x67,
	NUMPAD_8	= 0x68,
	NUMPAD_9	= 0x69,
	MILTIPLY	= 0x6A,
	ADD			= 0x6B,
	SEPARATOR	= 0x6C,
	SUBTRACT	= 0x6D,
	DECIMAL		= 0x6E,
	DIVIDE		= 0x6F,

	F1			= 0x70,
	F2			= 0x71,
	F3			= 0x72,
	F4			= 0x73,
	F5			= 0x74,
	F6			= 0x75,
	F7			= 0x76,
	F8			= 0x77,
	F9			= 0x78,
	F10			= 0x79,
	F11			= 0x7A,
	F12			= 0x7B,
	NUMLOC		= 0x90,

	MAX_KEY_NUM = 0xFF
};

/*	Class responsible for user input. Handles touches, clicks and key events */
class Input {
public:
	/* Represent user input Action like touch or click */
	struct Action {
		/* Action position on the screen */
		Vector2D pos;
		/* On mobile devices used for fingers count, on desktop used for mouse buttons, 0 - lmb, 1 - rmb */
		S32 id;
	};

	/* Occurs when user action started */
	Event<Action> ActionDown;
	/* Occurs when user action changed its position */
	Event<Action> ActionMove;
	/* Occurs when user action released */
	Event<Action> ActionUp;

	/* Occurs when keyboard button pressed */
	Event<Key> KeyPressed;
	/* Occurs when keyboard button released */
	Event<Key> KeyReleased;
	/* Occurs when char must be placed, for ex multiple chars can be placed by long press key button in some platforms */
	Event<char> CharEnter;

	/* Occurs when user scroll whether with a mouse wheel or touchpad gesture */
	Event<float> Scroll;
	/* Reflects current mouse position on the screen */
	Vector2D MousePosition;
	/* Checks if specific key pressed */
	bool IsPressed(Key key) const;
	/* Enables disables all touches */
	void TouchEnabled(bool enabled);

	/* Occurs instantly when user action started. Can be even in other thread. Do not use this */
	Event<float, float, S32> TargetActionDown;
	/* Occurs instantly when user action changed its position. Can be even in other thread. Do not use this */
	Event<float, float, S32> TargetActionMove;
	/* Occurs instantly when user action released. Can be even in other thread. Do not use this */
	Event<float, float, S32> TargetActionUp;

	void ResetKeys();

engineonly:
	Input();

	void Update();

protected:
	std::mutex input_mutex;
	bool touch_enabled			= true;
	Array<bool> pressed_keys	= Array<bool>((U32)Key::MAX_KEY_NUM, false);
	List<pair<Input::Action, int> > action_stack;

	Vector2D TargetToWordConvert(float x, float y) const;

	void TargetActionDownHandle(float x, float y, S32 actionID);
	void TargetActionMoveHandle(float x, float y, S32 actionID);
	void TargetActionUpHandle(float x, float y, S32 actionID);
	void KeyPressedHandle(Key key);
	void KeyReleasedHandle(Key key);
};

}
