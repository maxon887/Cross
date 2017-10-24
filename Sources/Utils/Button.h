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
#include "Input.h"
#include "Utils/Sprite.h"

namespace cross {

class Button {
public:
	Event<> Clicked;

	Button(Rect area);
	Button(Vector2D location);
	Button();
	~Button();

	void Update(float sec);

	void SetLocation(Vector2D location);
	void SetImages(Sprite* up, Sprite* down);
	void SetImages(Sprite* up);
	void SetSounds(Sound* push, Sound* pull);
	void SetColor(const Color& color);
	void Scale(float coef);
	void SetVisible(bool visible);
	bool IsPressed() const;
	float GetWidth() const;
	float GetHeight() const;
	Sprite* GetUpImage() const;
	Sprite* GetDownImage() const;
	Rect GetRect() const;
	Vector2D GetCenter() const;
	void Locate(Vector2D loc, float width, float heiht);
	void Locate(Rect rect);
	bool OnLocation(Vector2D p) const;

protected:
	//mandatory parameters
	Vector2D location		= Vector2D();
	bool located			= false;
	bool is_pressed			= false;
	S32 handled_action_id	= -1;
	Rect area				= Rect();
	float def_width			= 0;
	float def_height		= 0;
	//optional parameters
	Sprite* up_image		= NULL;
	Sprite* down_image		= NULL;
	Sound* push_sound		= NULL;
	Sound* pull_sound		= NULL;
	Color color				= Color::White;
	string label_text		= string();
	Vector2D text_size		= Vector2D();
	bool visible			= true;
	bool enable = true;

	void FitText(string text);

    U64 down_del;
    U64 up_del;
	void ActionDownHandler(Input::Action action);
	void ActionUpHandler(Input::Action action);
};
    
}
