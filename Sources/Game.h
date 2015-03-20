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

class Launcher;
class Graphics;
class Screen;

class Game{
public:
	float scale_factor;
	float width;
	float height;

	Launcher* launcher;
	Graphics* graphics;

	virtual Screen* GetStartScreen() = 0;

	Game(Launcher* launcher, float width, float height);
	void Start();
	void SetScreen(Screen* screen);
	Screen* GetCurrentScreen();
	virtual ~Game();
private:
	Screen* current_screen;
};