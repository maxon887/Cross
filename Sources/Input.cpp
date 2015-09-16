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
#include "Game.h"

using namespace cross;

Input::Input(Game* game){
	this->game = game;
	input_state = false;
	game->launcher->LogIt("Input initialized");
}

bool Input::HaveInput(){
	return input_state;
}

Point Input::GetInput(){
	Point ret;
	ret.x = input_loc.x / game->GetScaleFactor();
	ret.y = input_loc.y / game->GetScaleFactor();
	return ret;
}
void Input::PressKey(Key key){
	TRIGGER_EVENT(KeyPressed, key);
}

void Input::ReleaseKey(Key key){
	TRIGGER_EVENT(KeyReleased, key);
}
