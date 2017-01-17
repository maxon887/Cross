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
#include "UI.h"
#include "Input.h"

namespace cross {

class Button : public UI{
public:
	DECLARE_EVENT(void) Clicked;

	Button(Rect area, string text);
	Button(Vector2D location, string text);
	Button(string text);
	Button(string text, Font* font);
	Button(Rect area);
	Button(Vector2D location);
	Button();
	~Button();

	virtual void Update(float sec);

	void SetLocation(Vector2D location);
	void SetText(string text);
	void SetImages(Sprite* up, Sprite* down);
	void SetImages(Sprite* up);
	void SetSounds(Sound* push, Sound* pull);
	void Scale(float coef);
	void SetVisible(bool visible);
	bool IsPressed() const;
	float GetWidth() const;
	float GetHeight() const;
	Sprite* GetUpImage() const;
	Sprite* GetDownImage() const;
	Rect GetRect() const;
	Vector2D GetCenter() const;
protected:
	//mandatory parameters
	Vector2D location;
	bool located;
	bool is_pressed;
	S32 handled_action_id;
	Rect area;
	float def_width;
	float def_height;
	bool visible;
	//optional parameters
	Sprite* up_image;
	Sprite* down_image;
	Sound* push_sound;
	Sound* pull_sound;
	Font* font;
	string label_text;
	Vector2D text_size;

	void Locate(Vector2D loc, float width, float heiht);
	void Locate(Rect rect);
	void FitText(string text);
	//connections
	FastDelegate1<Input::Action, void> action_down_delegate;
	FastDelegate1<Input::Action, void> action_up_delegate;

	void ActionDownHandler(Input::Action action);
	void ActionUpHandler(Input::Action action);
};
    
}
