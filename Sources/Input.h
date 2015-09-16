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

#include "Point.h"
#include "Launcher.h"
#include "Events\Event.h"

namespace cross {

class Game;

enum class Key{
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
	SHIFT		= 10
};

/*	Class responsible for user input. 
	Handle touches, clicks and some key events */
class Input{
public:
	/* Return true if user have made touch/click */
	bool HaveInput();
	/* Return last user touch/click position */
	Point GetInput();

	DECLARE_EVENT(void, Key) KeyPressed;
	DECLARE_EVENT(void, Key) KeyReleased;
//Internal data. You don't need call any of this methods or modify variable
public:
	Input(Game* game);
	void PressKey(Key key);
	void ReleaseKey(Key key);
	bool input_state;
	Point input_loc;
private:
	Game* game;
};

}
