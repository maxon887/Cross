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
#include "Graphics.h"
#include "Input.h"
#include "Image.h"

class Button{
//User module
public:
	Button(Game* game, PointX location, Image* release, Image* pressed);
	void Update();
	void RegisterCallback(function<void()> callback);
//Framework module. You don't need call any of this methods or modify variable
protected:
	Launcher* launcher;
	Graphics* graphics;
	Input* input;
	PointX location;
	Image* release;
	Image* pressed;
private:
	bool OnLocation(float x, float y);
	PointX* press_loc;
	function<void()> callback;
	bool callback_registered;
};
