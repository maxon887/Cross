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

//#define CROSSDEBUG

#include <time.h>

class Launcher;
class Graphics;
class Input;
class Screen;
class Debuger;

class Game{
//User module
public:
	Launcher* launcher;
	Graphics* graphics;
	Input* input;
	/* This method must return first game Screen */
	virtual Screen* GetStartScreen() = 0;
	/* Constructor gets virtual game width
	   don't mess with target width.
	   Height will be set automatically depend on screen aspect ratio. */
	Game(Launcher* launcher, float width);

	/* Reflect ratio between game and target device width.*/
	float GetScaleFactor();
	/* Game width */
	float GetWidth();
	/* Game height; */
	float GetHeight();
	/* Start the game. Calls automaticaly */
	void Start();
	/* Set up new game Screen. Previous creen data will be lost. */
	void SetScreen(Screen* screen);
	/* Returns current game screen. */
	Screen* GetCurrentScreen();
	virtual ~Game();
protected:
	float width;
	float height;
	float scale_factor;
//Framework module. You don't need call any of this methods or modify variable
public:
    void Update();
private:
	Screen* current_screen;
    clock_t render_time;
#ifdef CROSSDEBUG
    Debuger* debuger;
#endif
};
