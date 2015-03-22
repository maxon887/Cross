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
	/* Reflect ratio between game and target device width.*/
	float scale_factor;
	/* Game width */
	float width;
	/* Game height; */
	float height;

	Launcher* launcher;
	Graphics* graphics;
	/* This method must return first game Screen */
	virtual Screen* GetStartScreen() = 0;
	/* Constructor gets virtual game width and height
	   don't mess with target width and height. */
	Game(Launcher* launcher, float width, float height);
	/* Start the game. Calls automaticaly */
	void Start();
	/* Set up new game Screen. Previous creen data will be lost. */
	void SetScreen(Screen* screen);
	/* Returns current game screen. */
	Screen* GetCurrentScreen();
	virtual ~Game();
private:
	Screen* current_screen;
};
