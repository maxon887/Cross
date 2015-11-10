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
#include "Screen.h"

#include <chrono>
#include <mutex>

namespace cross{
    
class Debuger;

extern mutex global_mutex;

/*	Core game class. Every game must inherit that class.
	Also provide interfaces for all virtual modules. Like graphics, input, sound etc. */
class Game{
public:
	/* You need to override this method to get engine know from which screen it must start */
	virtual Screen* GetStartScreen() = 0;
	/* Game constructor. Virtual word coordinates will match target physical pixels */
	Game(Launcher* launcher);
	/* Game constructor. Virtual world height will calculate proportionally physical screen ratio */
	Game(Launcher* launcher, float width);
	/* Game constructor. Game screen will scroll automatically */
	Game(Launcher* launcher, float width, float height);
	// Cause when game is about to start
	virtual void Start();
	// Cause when game needs to be paused. For example input call or window lost focus
	virtual void Suspend();
	// Cause when game gained focus.
	virtual void Resume();
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
	bool Is3D();
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
    chrono::time_point<chrono::high_resolution_clock> render_time;
	void Init(Launcher* launch);
};
    
}
