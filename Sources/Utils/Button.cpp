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
	this->push = NULL;
	this->pull = NULL;
	this->input = game->input;
	this->location = Point();
	this->up = up;
	this->down = down;
	this->have_area = false;
	this->is_pressed = false;
	this->active = true;
	this->area.width = up->GetWidth();
	this->area.height = up->GetHeight();
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::Button(Game* game, Point location, Image* up, Image* down){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->push = NULL;
	this->pull = NULL;
	this->input = game->input;
	this->location = location;
	this->up = up;
	this->down = down;
	this->active = true;
	this->have_area = false;
	this->is_pressed = false;
	InitRect(location, up->GetWidth(), up->GetHeight());
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::Button(Game* game, float width, float height){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->push = NULL;
	this->pull = NULL;
	this->input = game->input;
	this->location = Point();
	this->area = area;
	this->up = NULL;
	this->down = NULL;
	this->active = true;
	this->is_pressed = false;
	InitRect(location, width, height);
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::Button(Game* game, Rect area){
	this->launcher = game->launcher;
	this->graphics = game->graphics;
	this->push = NULL;
	this->pull = NULL;
	this->input = game->input;
	this->location = Point();
	this->area = area;
	this->up = NULL;
	this->down = NULL;
	this->active = true;
	this->is_pressed = false;
	this->have_area = true;
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::~Button(){
	graphics->ReleaseImage(down);
	graphics->ReleaseImage(up);
	input->ActionDown -= action_down_delegate;
	input->ActionUp -= action_up_delegate;
}

void Button::SetLocation(Point location){
	this->location = location;
	InitRect(location, area.width, area.height);
}

void Button::SetActive(bool active){
	this->active = active;
}

void Button::SetSounds(Audio* push, Audio* pull){
	this->push = push;
	this->pull = pull;
}

void Button::Update(){
	if(is_pressed){
		if(down != NULL){
			graphics->DrawImage(location, down);
		}
	}else{
		if(up != NULL){
			graphics->DrawImage(location, up);
		}
	}
}

float Button::GetWidth(){
	return up->GetWidth();
}

float Button::GetHeight(){
	return up->GetHeight();
}

Rect Button::GetRect(){
	if(!have_area){
		throw string("This button have not area");
	}
	return area;
}

Point Button::GetCenter(){
	return location;
}

bool Button::OnLocation(Point p){
	return OnLocation(p.x, p.y);
}

bool Button::OnLocation(float x, float y){
	return	x > area.x &&
			x < (area.x + area.width) &&
			y > area.y &&
			y < (area.y + area.height);
}

void Button::DrawUp(){
	graphics->DrawImage(location, up);
}
void Button::DrawDown(){
	graphics->DrawImage(location, down);
}

void Button::InitRect(Point loc, float width, float heiht){
	area.x = loc.x - width / 2.f;
	area.y = loc.y - heiht / 2.f;
	area.width = width;
	area.height = heiht;
	have_area = true;
}

bool Button::IsPressed(){
	return is_pressed;
}

void Button::SetPressed(bool pressed){
	is_pressed = pressed;
}

void Button::ActionDownHandler(Point pos){
	if(active && OnLocation(pos.x, pos.y)){
		is_pressed = true;
		if(push != NULL){
			push->Play();
		}
	}
}

void Button::ActionUpHandler(Point pos){
	if(active){
		if(is_pressed && push != NULL){
			push->Play();
		}
		is_pressed = false;
		if(OnLocation(pos.x, pos.y)){
			is_pressed = false;
			if(down != NULL){
				graphics->DrawImage(location, down);
			}
			if(pull != NULL){
				pull->Play();
			}
			TRIGGER_EVENT(Clicked);
			return;
		}
	}
}