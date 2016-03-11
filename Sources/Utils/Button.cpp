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
	label_text = text;
	if(text.size()) {
		text_size.x = gfx2D->GetDefaultFont()->GetCharWidth() * text.size();
		text_size.y = gfx2D->GetDefaultFont()->GetSize();
	}
	SetRect(area);
}

Button::Button(Vector2D location, string text) :
	Button()
{
	label_text = text;
	if(text.size()) {
		text_size.x = gfx2D->GetDefaultFont()->GetCharWidth() * text.size();
		text_size.y = gfx2D->GetDefaultFont()->GetSize();
	}
	SetRect(location, text_size.x + HORIZONTAL_PADDING, text_size.y + VERTICAL_PADDING);
}

Button::Button(string text) :
	Button()
{
	label_text = text;
	if(text.size()) {
		text_size.x = gfx2D->GetDefaultFont()->GetCharWidth() * text.size();
		text_size.y = gfx2D->GetDefaultFont()->GetSize();
	}
	area.width = text_size.x + HORIZONTAL_PADDING;
	area.height = text_size.y + VERTICAL_PADDING;
}

Button::Button(Rect area) :
	Button()
{
	SetRect(area);
}

Button::Button(Vector2D location) :
	Button()
{
	SetRect(location, HORIZONTAL_PADDING, VERTICAL_PADDING);
}

Button::Button() :
	located(false),
	is_pressed(false),
	active(true),
	push_sound(nullptr),
	pull_sound(nullptr),
	up_image(nullptr),
	down_image(nullptr)
{
	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;
}

Button::~Button() {
	if (down_image != nullptr) {
		gfx2D->ReleaseSprite(down_image);
	}
	if (up_image != nullptr) {
		gfx2D->ReleaseSprite(up_image);
	}

	input->ActionDown -= action_down_delegate;
	input->ActionUp -= action_up_delegate;
	//release audio?
}

void Button::Update() {
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


	if(label_text.size()) {
		gfx2D->DrawText(Vector2D(area.x + area.width / 2 - text_size.x / 2,
			area.y + area.height / 2 - text_size.y / 2 + 10), label_text, font);
	}
}

void Button::SetLocation(Vector2D location) {
	SetRect(location, area.width, area.height);
}

void Button::SetText(string text) {
	label_text = text;

	if(text.size()) {
		text_size.x = gfx2D->GetDefaultFont()->GetCharWidth() * text.size();
		text_size.y = gfx2D->GetDefaultFont()->GetSize();
	}
}

void Button::SetImages(Sprite * up, Sprite * down)
{
	this->up_image = up;
	this->down_image = down;

	if(up != nullptr) {
		area.width = up->GetWidth();
		area.height = up->GetHeight();
	}
}

void cross::Button::SetImages(Sprite * up)
{
	SetImages(up, nullptr);
}

void Button::SetSounds(Audio* push, Audio* pull) {
	this->push_sound = push;
	this->pull_sound = pull;
}

void Button::SetActive(bool active) {
	this->active = active;
}

void Button::Scale(float scale){
	area.width *= scale;
	area.height *= scale;
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

bool Button::IsOnLocation(Vector2D p) {
	return IsOnLocation(p.x, p.y);
}

void cross::Button::FitText()
{
}

bool Button::IsOnLocation(float x, float y) {
	return	x > area.x &&
		x < (area.x + area.width) &&
		y > area.y &&
		y < (area.y + area.height);
}

void Button::SetRect(Vector2D loc, float width, float heiht) {
	this->location = loc;
	area.x = loc.x - width / 2.f;
	area.y = loc.y - heiht / 2.f;
	area.width = width;
	area.height = heiht;
	located = true;
}

void Button::SetRect(Rect rect){
	location.x = rect.x + rect.width / 2.f;
	location.y = rect.y + rect.height / 2.f;
	this->area = rect;
	located = true;
}

void Button::ActionDownHandler(Vector2D pos) {
	if (active && IsOnLocation(pos.x, pos.y)) {
		is_pressed = true;
		if (push_sound != nullptr) {
			push_sound->Play();
		}
	}
}

void Button::ActionUpHandler(Vector2D pos) {
	if (active && is_pressed) {
		is_pressed = false;
		if (push_sound != nullptr) {
			push_sound->Play();
		}
		if (IsOnLocation(pos.x, pos.y)) {
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