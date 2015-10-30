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
	
#include "ToggleButton.h"

using namespace cross;

ToggleButton::ToggleButton(Game* game, Image* on, Image* off)
	:Button(game, on, off){
	this->on = on;
	this->off = off;
	this->location = Point();
	this->state = true;
}

ToggleButton::ToggleButton(Game* game, Point location, Image* on, Image* off)
	:Button(game, location, on, off){
	this->on = on;
	this->off = off;
	this->state = true;
}

void ToggleButton::Update(){
	if(state)
		graphics->DrawImage(location, on);
	else
		graphics->DrawImage(location, off);

}

bool ToggleButton::GetState(){
	return state;
}

void ToggleButton::SetState(bool state){
	this->state = state;
}