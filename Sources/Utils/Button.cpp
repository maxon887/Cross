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
	
#include "Button.h"

using namespace cross;

Button::Button(Game* game, Image* up, Image* down){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = Point();
	this->up = up;
	this->down = down;
	this->press_loc = NULL;
	this->callback_registered = false;
}

Button::Button(Game* game, Point location, Image* up, Image* down){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->input = game->input;
	this->location = location;
	this->up = up;
	this->down = down;
	this->press_loc = NULL;
	this->callback_registered = false;
}

void Button::SetLocation(Point location){
	this->location = location;
}

void Button::Update(){
	if(input->HaveInput() && press_loc != NULL && OnLocation(press_loc->x, press_loc->y)){
		graphics->DrawImage(location, down);
	}else{
		graphics->DrawImage(location, up);
	}
	//first press
	if(press_loc == NULL && input->HaveInput()){
		press_loc = new Point(input->GetInput().x, input->GetInput().y);
	}
	//callback
	if(!input->HaveInput() && press_loc != NULL){
		if(OnLocation(input->GetInput().x, input->GetInput().y)){
			if(callback_registered){
				delete press_loc;
				press_loc = NULL;
				callback();
				return;
			}else{
				launcher->LogIt("Callback not registered");
			}
		}
	}
	if(!input->HaveInput()){
		delete press_loc;
		press_loc = NULL;
	}
}

float Button::GetWidth(){
	return up->GetWidth();
}

float Button::GetHeight(){
	return up->GetHeight();
}

Rect Button::GetRect(){
	Rect rect;
	rect.x = location.x - up->GetWidth() / 2;
	rect.y = location.y - up->GetHeight() / 2;
	rect.width = up->GetWidth();
	rect.height = up->GetHeight();
	return rect;
}

Point Button::GetCenter(){
	return location;
}

bool Button::OnLocation(float x, float y){
		return  x > (location.x - up->GetWidth() / 2) &&
				x < (location.x + up->GetWidth() / 2) &&
				y > (location.y - up->GetHeight() / 2) &&
				y < (location.y + up->GetHeight() / 2);
}

void Button::RegisterCallback(function<void()> callback){
	callback_registered = true;
	this->callback = callback;
}

Button::~Button(){
	graphics->ReleaseImage(down);
	graphics->ReleaseImage(up);
}
