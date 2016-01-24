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

#define DEFAULT_WIDTH 100
#define DEFAULT_HEIGHT 100
#define DEFAULT_LOCATION 0
#define NO_TEXT ""

using namespace cross;

Button::Button(Rect area, string text) :
	push_sound(nullptr),
	pull_sound(nullptr),
	up_image(nullptr),
	down_image(nullptr),
	have_area(false),
	is_pressed(false),
	active(true),
	is_with_text(false),
	label_text(text)
{
	this->area.width = area.width;
	this->area.height = area.height;

	up_image = gfx2D->LoadImage("DefaultButton.png");

	action_down_delegate = MakeDelegate(this, &Button::ActionDownHandler);
	action_up_delegate = MakeDelegate(this, &Button::ActionUpHandler);
	input->ActionDown += action_down_delegate;
	input->ActionUp += action_up_delegate;

	if (label_text.compare(NO_TEXT))
		is_text_resizable = false;
	else
		is_text_resizable = true;

}

Button::Button(Vector2D location, string text) :
	Button(Rect(location.x, location.y, DEFAULT_WIDTH, DEFAULT_HEIGHT), text)
{
	area.width = gfx2D->DrawText(Vector2D(area.x, area.y), text);
	area.height = gfx2D->GetDefaultFont()->GetSize();
	is_with_text = true;
}

Button::Button(int locX, int locY, string text) :
	Button(Rect(locX, locY, DEFAULT_WIDTH, DEFAULT_HEIGHT), text)
{
	is_with_text = true;
}

Button::Button(Vector2D location) :
	Button(Rect(location.x, location.y, DEFAULT_WIDTH, DEFAULT_HEIGHT), NO_TEXT)
{
}

Button::Button(int locX, int locY) :
	Button(Rect(locX, locY, DEFAULT_WIDTH, DEFAULT_HEIGHT), NO_TEXT)
{
}

Button::Button(Rect area) :
	Button(area, NO_TEXT)
{
}

cross::Button::Button(Sprite * upImage, Sprite * downImage) :
	Button(Rect(DEFAULT_LOCATION, DEFAULT_LOCATION, DEFAULT_WIDTH, DEFAULT_HEIGHT), NO_TEXT)
{
	SetImages(upImage, downImage);
}

Button::~Button() {
	if (down_image) {
		gfx2D->ReleaseImage(down_image);
	}
	gfx2D->ReleaseImage(up_image);
	input->ActionDown -= action_down_delegate;
	input->ActionUp -= action_up_delegate;
	//release audio?
}

void Button::SetLocation(Vector2D location) {
	this->location = location;
	InitRect(location, area.width, area.height);
}

void cross::Button::SetRect(Rect area)
{
	this->area = area;
}

void Button::SetActive(bool active) {
	this->active = active;
}

void Button::SetSounds(Audio* push, Audio* pull) {
	this->push_sound = push;
	this->pull_sound = pull;
}

void Button::SetImages(Sprite * up, Sprite * down)
{
	this->up_image = up;
	this->down_image = down;

	if (up != nullptr)
		InitRect(location, up->GetWidth(), up->GetHeight());
}

Sprite* Button::GetUpImage(){
	return up_image;
}

Sprite* Button::GetDownImage(){
	return down_image;
}

void Button::Update() {
	if (is_pressed) {
		if (down_image != nullptr) {
			gfx2D->DrawSprite(location, down_image);
		}
	}
	else {
		if (up_image != nullptr) {
			gfx2D->DrawSprite(location, up_image);
		}
	} 
	if (!is_text_resizable) {
		gfx2D->DrawSprite(Vector2D(area.x + area.width / 2, area.y + area.height / 2), up_image);
	}

	if (is_with_text) {
		gfx2D->DrawText(Vector2D(area.x, area.y), label_text);
	}
	gfx2D->DrawRect(area, Color::Blue);
} 

float Button::GetWidth() {
	return area.width;
	//return up_image->GetWidth();
}

float Button::GetHeight() {
	return area.height;
//	return up_image->GetHeight();
}

Rect Button::GetRect() {
	if (!have_area) {
		throw CrossException("This button does have not area");
	}
	return area;
}

Vector2D Button::GetCenter() {
	return location;
}

bool Button::OnLocation(Vector2D p) {
	return OnLocation(p.x, p.y);
}

bool Button::OnLocation(float x, float y) {
	if (!have_area) {
		throw CrossException("This button does have not area");
	}
	return	x > area.x &&
		x < (area.x + area.width) &&
		y > area.y &&
		y < (area.y + area.height);
}

void Button::DrawUp() {
	gfx2D->DrawSprite(location, up_image);
}
void Button::DrawDown() {
	gfx2D->DrawSprite(location, down_image);
}

void Button::InitRect(Vector2D loc, float width, float heiht) {
	area.x = loc.x - width / 2.f;
	area.y = loc.y - heiht / 2.f;
	area.width = width;
	area.height = heiht;
	have_area = true;
}

bool Button::IsPressed() {
	return is_pressed;
}

void Button::SetPressed(bool pressed) {
	is_pressed = pressed;
}

void Button::ActionDownHandler(Vector2D pos) {
	if (active && OnLocation(pos.x, pos.y)) {
		is_pressed = true;
		if (push_sound != nullptr) {
			push_sound->Play();
		}
	}
}

void Button::ActionUpHandler(Vector2D pos) {
	if (active) {
		if (is_pressed && push_sound != nullptr) {
			push_sound->Play();
		}
		is_pressed = false;
		if (OnLocation(pos.x, pos.y)) {
			is_pressed = false;
			if (down_image != nullptr) {
				gfx2D->DrawSprite(location, down_image);
			}
			if (pull_sound != nullptr) {
				pull_sound->Play();
			}
			TRIGGER_EVENT(Clicked);
			return;
		}
	}
}