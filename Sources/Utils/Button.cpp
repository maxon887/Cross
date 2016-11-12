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
#include "Font.h"
#include "Launcher.h"

#define HORIZONTAL_PADDING 50.f
#define VERTICAL_PADDING 25.f

using namespace cross;


Button::Button(Rect area, string text) :
	Button()
{
	Locate(area);
	font = gfx2D->GetDefaultFont()->Clone();
	FitText(text);
}

Button::Button(Vector2D location, string text) :
	Button()
{
	font = gfx2D->GetDefaultFont()->Clone();
	FitText(text);
	Locate(location, text_size.x + HORIZONTAL_PADDING, text_size.y + VERTICAL_PADDING);
}

Button::Button(string text) :
	Button()
{
	font = gfx2D->GetDefaultFont()->Clone();
	FitText(text);
}

Button::Button(Rect area) :
	Button()
{
	Locate(area);
}

Button::Button(Vector2D location) :
	Button()
{
	Locate(location, HORIZONTAL_PADDING, VERTICAL_PADDING);
}

Button::Button() :
	located(false),
	is_pressed(false),
	active(true),
	push_sound(nullptr),
	pull_sound(nullptr),
	up_image(nullptr),
	down_image(nullptr),
	font(nullptr),
	def_width(0),
	def_height(0),
	handled_action_id(-1)
{
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::~Button() {
	if (down_image != nullptr) {
		delete down_image;
	}
	if (up_image != nullptr) {
		delete up_image;
	}
	if (font != nullptr) {
		delete font;
	}
	if(push_sound != nullptr) {
		delete push_sound;
	}
	if(pull_sound != nullptr) {
		delete pull_sound;
	}

	input->ActionDown -= action_down_delegate;
	input->ActionUp -= action_up_delegate;
}

void Button::Update(float sec) {
	if(!located) {
		throw CrossException("Button must be located first");
	}

	if(is_pressed) {
		if(down_image != nullptr) {
			gfx2D->DrawSprite(location, down_image);
		} else if(up_image == nullptr) {
			gfx2D->DrawRect(area, Color::Red, true);
		}
	} else {
		if(up_image != nullptr) {
			gfx2D->DrawSprite(location, up_image);
		} else {
			gfx2D->DrawRect(area, Color::Blue, true);
		}
	}


	if(!label_text.empty()) {
		Vector2D textPos;
		textPos.x = area.x + area.width / 2 - text_size.x / 2;
		textPos.y = area.y + area.height / 2 - text_size.y / 2;
		gfx2D->DrawText(textPos, label_text, font);
	}
	
	//debug active area
	gfx2D->DrawRect(area, Color::Red);
}

void Button::SetLocation(Vector2D location) {
	Locate(location, area.width, area.height);
}

void Button::SetText(string text) {
	if(!font){
		font = gfx2D->GetDefaultFont()->Clone();
	}
	FitText(text);
}

void Button::SetImages(Sprite * up, Sprite * down) {
	this->up_image = up;
	this->down_image = down;

	if(up != nullptr) {
		Locate(GetCenter(), up->GetWidth(), up->GetHeight());
	}
}

void Button::SetImages(Sprite * up) {
	SetImages(up, nullptr);
}

void Button::SetSounds(Audio* push, Audio* pull) {
	this->push_sound = push;
	this->pull_sound = pull;
}

void Button::SetActive(bool active) {
	this->active = active;
}

void Button::Scale(float scale) {
	area.width = def_width * scale;
	area.height = def_height * scale;
	if(up_image){
		up_image->SetScale(scale);
	}
	if(down_image){
		down_image->SetScale(scale);
	}
}

bool Button::IsPressed() const {
	return is_pressed;
}

float Button::GetWidth() const {
	return area.width;
}

float Button::GetHeight() const {
	return area.height;
}

Sprite* Button::GetUpImage() const {
	return up_image;
}

Sprite* Button::GetDownImage() const {
	return down_image;
}


Rect Button::GetRect() const {
	return area;
}

Vector2D Button::GetCenter() const {
	return location;
}

void Button::FitText(string text) {
	label_text = text;
	/*
	if (!label_text.empty()) {
		text_size.x = gfx2D->GetDefaultFont()->GetCharWidth() * label_text.size();
		text_size.y = gfx2D->GetDefaultFont()->GetSize();
	}*/

	if (!located) return;

	/*
	if (text_size.x > area.width - HORIZONTAL_PADDING) {
		//font->SetSize(
		//Font::SizeForWidthForStringLength(area.width - HORIZONTAL_PADDING, label_text.size())
		//	);
		throw CrossException("Not Implemented");
	}*/

	if (!label_text.empty()) {
		text_size.x = font->GetCharWidth() * label_text.size();
		text_size.y = font->GetSize();
	}
	
}

void Button::Locate(Vector2D loc, float width, float height) {
	this->location = loc;
	area.x = loc.x - width / 2.f;
	area.y = loc.y - height / 2.f;
	area.width = width;
	area.height = height;
	if(def_width == 0 && def_height == 0){
		def_width = width;
		def_height = height;
	}
	located = true;
	FitText(label_text);
}

void Button::Locate(Rect rect){
	location.x = rect.x + rect.width / 2.f;
	location.y = rect.y + rect.height / 2.f;
	if(def_width == 0 && def_height == 0){
		def_width = rect.width;
		def_height = rect.height;
	}
	this->area = rect;
	located = true;
	FitText(label_text);
}

void Button::ActionDownHandler(Input::Action action) {
	if(handled_action_id == -1){
		if (active && OnLocation(action.pos)) {
			is_pressed = true;
			handled_action_id = action.id;
			if (push_sound != nullptr) {
				push_sound->Play();
			}
		}
	}
}

void Button::ActionUpHandler(Input::Action action) {
	if(handled_action_id == action.id){
		if (active && is_pressed) {
			is_pressed = false;
			handled_action_id = -1;
			if (push_sound != nullptr) {
				push_sound->Play();
			}
			if (OnLocation(action.pos)) {
				if (down_image != nullptr) {
					gfx2D->DrawSprite(location, down_image);
				}
				if (pull_sound != nullptr) {
					pull_sound->Play();
				}
				TRIGGER_EVENT(Clicked);
			}
		}
	}
}
