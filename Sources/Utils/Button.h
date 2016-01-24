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
#include "Cross.h"
#include "Events\Event.h"

namespace cross {

class Button{
public:

	Button(Vector2D location, string text);
	Button(int locX, int locY, string text);
	Button(Vector2D location);
	Button(int locX, int locY);
	Button(Rect area);
	Button(Sprite* upImage, Sprite* downImage);


	Button::Button(Rect area, string text);


	~Button();

	void Update();
	void SetSounds(Audio* push, Audio* pull);
	void SetImages(Sprite* up, Sprite* down);
	Sprite* GetUpImage();
	Sprite* GetDownImage();
	void SetLocation(Vector2D location);
	void SetRect(Rect area);
	void SetActive(bool active);
	float GetWidth();
	float GetHeight();
	bool IsPressed();
	void SetPressed(bool pressed);
	bool OnLocation(float x, float y);
	bool OnLocation(Vector2D p);
	void DrawUp();
	void DrawDown();
	Rect GetRect();
	Vector2D GetCenter();

	DECLARE_EVENT(void) Clicked;
protected:
	Vector2D location;
	Rect area;
	Sprite* up_image;
	Sprite* down_image;
	Audio* push_sound;
	Audio* pull_sound;
	string label_text;
	void InitRect(Vector2D loc, float width, float heiht);
	bool is_pressed;
	bool have_area;
	bool active;
	bool is_with_text;
	bool is_text_resizable;

	FastDelegate1<Vector2D, void> action_down_delegate;
	FastDelegate1<Vector2D, void> action_up_delegate;

	void ActionDownHandler(Vector2D pos);
	void ActionUpHandler(Vector2D pos);
};
    
}
