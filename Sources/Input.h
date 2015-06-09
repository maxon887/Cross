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

namespace cross {

class Game;

enum class Key{
	UNDEFINED,
	PAUSE
};

/*	Class responsible for user input. 
	Handle touches, clicks and some key events */
class Input{
public:
	/* Return true if user have made touch/click */
	bool HaveInput();
	/* Return last user touch/click position */
	Point GetInput();
	/* Return true if user have pushed the key */
	bool HaveKey();
	/* Return last user pushed key */
	Key GetKey();
//Internal data. You don't need call any of this methods or modify variable
public:
	Input(Game* game);
	bool input_state;
	Point input_loc;
	bool key_state;
	Key key_key;
private:
	Game* game;
};
    
}
