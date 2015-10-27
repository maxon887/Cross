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

#include "Game.h"

//#include <functional>

namespace cross {

class Button{
public:
	Button(Game* game, Image* up, Image* down);
	Button(Game* game, Point location, Image* up, Image* down);
	Button(Game* game, float width, float height);
	Button(Game* game, Rect area);
	~Button();
	void Update();
	void SetSounds(Audio* push, Audio* pull);
	void SetLocation(Point location);
	void SetActive(bool active);
	float GetWidth();
	float GetHeight();
	bool IsPressed();
	void SetPressed(bool pressed);
	bool OnLocation(float x, float y);
	bool OnLocation(Point p);
	void DrawUp();
	void DrawDown();
	Rect GetRect();
	Point GetCenter();

	DECLARE_EVENT(void) Clicked;
protected:
	Launcher* launcher;
	Graphics* graphics;
	Input* input;
	Point location;
	Rect area;
	Image* up;
	Image* down;
	Audio* push;
	Audio* pull;
	void InitRect(Point loc, float width, float heiht);
	bool is_pressed;
	bool have_area;
	bool active;

	FastDelegate1<Point, void> action_down_delegate;
	FastDelegate1<Point, void> action_up_delegate;

	void ActionDownHandler(Point pos);
	void ActionUpHandler(Point pos);
};
    
}
