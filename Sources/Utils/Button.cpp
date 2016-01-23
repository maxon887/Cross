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
#include "Audio.h"
#include "Graphics2D.h"
#include "Input.h"
#include "Sprite.h"

using namespace cross;

Button::Button(Sprite* up, Sprite* down){
	this->pushSound = nullptr;
	this->pullSound = nullptr;
	this->upImage = up;
	this->downImage = down;
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

Button::Button(Vector2D location, Sprite* up, Sprite* down){
	this->pushSound = nullptr;
	this->pullSound = nullptr;
	this->location = location;
	this->upImage = up;
	this->downImage = down;
	this->active = true;
	this->have_area = false;
	this->is_pressed = false;
	InitRect(location, up->GetWidth(), up->GetHeight());
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::Button(float width, float height){
	this->pushSound = nullptr;
	this->pullSound = nullptr;
	this->area = area;
	this->upImage = nullptr;
	this->downImage = nullptr;
	this->active = true;
	this->is_pressed = false;
	this->have_area = false;
	InitRect(location, width, height);
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::Button(Rect area){
	this->pushSound = nullptr;
	this->pullSound = nullptr;
	this->area = area;
	this->upImage = nullptr;
	this->downImage = nullptr;
	this->active = true;
	this->is_pressed = false;
	this->have_area = true;
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::~Button(){
	if(downImage){
		gfx2D->ReleaseImage(downImage);
	}
	gfx2D->ReleaseImage(upImage);
	input->ActionDown -= action_down_delegate;
	input->ActionUp -= action_up_delegate;
}

void Button::SetLocation(Vector2D location){
	this->location = location;
	InitRect(location, area.width, area.height);
}

void Button::SetActive(bool active){
	this->active = active;
}

void Button::SetSounds(Audio* push, Audio* pull){
	this->pushSound = push;
	this->pullSound = pull;
}

void Button::Update(){
	if(is_pressed){
		if(downImage != nullptr){
			gfx2D->DrawSprite(location, downImage);
		}
	}else{
		if(upImage != nullptr){
			gfx2D->DrawSprite(location, upImage);
		}
	}
}

float Button::GetWidth(){
	return upImage->GetWidth();
}

float Button::GetHeight(){
	return upImage->GetHeight();
}

Rect Button::GetRect(){
	if(!have_area){
		throw CrossException("This button does have not area");
	}
	return area;
}

Vector2D Button::GetCenter(){
	return location;
}

bool Button::OnLocation(Vector2D p){
	return OnLocation(p.x, p.y);
}

bool Button::OnLocation(float x, float y){
	if(!have_area){
		throw CrossException("This button does have not area");
	}
	return	x > area.x &&
			x < (area.x + area.width) &&
			y > area.y &&
			y < (area.y + area.height);
}

void Button::DrawUp(){
	gfx2D->DrawSprite(location, upImage);
}
void Button::DrawDown(){
	gfx2D->DrawSprite(location, downImage);
}

void Button::InitRect(Vector2D loc, float width, float heiht){
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

void Button::ActionDownHandler(Vector2D pos){
	if(active && OnLocation(pos.x, pos.y)){
		is_pressed = true;
		if(pushSound != nullptr){
			pushSound->Play();
		}
	}
}

void Button::ActionUpHandler(Vector2D pos){
	if(active){
		if(is_pressed && pushSound != nullptr){
			pushSound->Play();
		}
		is_pressed = false;
		if(OnLocation(pos.x, pos.y)){
			is_pressed = false;
			if(downImage != nullptr){
				gfx2D->DrawSprite(location, downImage);
			}
			if(pullSound != nullptr){
				pullSound->Play();
			}
			TRIGGER_EVENT(Clicked);
			return;
		}
	}
}