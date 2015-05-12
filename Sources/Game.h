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

#define CROSSDEBUG

#ifdef _WIN32
#define WIN
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#define IOS true
#elif defined(ANDROID) || defined(__ANDROID__)
void* bind(void* func, void* sender);
//#define __cplusplus 201103L
//#undef ANDROID
//#define ANDROID
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <chrono>
using namespace std;
using namespace chrono;

#include "PointX.h"
#include "RectX.h"

class Launcher;
class Graphics;
class Input;
class Screen;
class Debuger;
class Saver;

/*	Core game class. Every game must inherit that class.
	Also provide interfaces for all virtual modules. Like graphics, input, sound etc. */
class Game{
public:
	Launcher* launcher;
	Graphics* graphics;
	Input* input;
	Saver* saver;
	/* You need to override this method to get engine know from which screen it must start */
	virtual Screen* GetStartScreen() = 0;
	/* Constructor gets virtual game width.
	   Don't mess with target width.
	   Game height will be set automatically depend on screen aspect ratio. */
	Game(Launcher* launcher, float width);
	virtual void Start();
	/* Reflect ratio between game and target device width.*/
	float GetScaleFactor();
	/* Return virtual game width */
	float GetWidth();
	/* Return virtual game height; */
	float GetHeight();
	/* Set up new Screen. Previous screen data will be deleted. */
	void SetScreen(Screen* screen);
	/* Returns current game screen. */
	Screen* GetCurrentScreen();
	/* Exit from application */
    void Exit();

//Internal data. You don't need call any of this methods or modify variable
public:
    void Update();
    Debuger* debuger;
	virtual ~Game();
protected:
	float width;
	float height;
	float scale_factor;
private:
	Screen* current_screen;
    time_point<high_resolution_clock> render_time;
};
