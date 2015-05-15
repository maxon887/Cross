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
#include "Apple.h"

enum GameState{
	RUNNING,
	ONREADY,
	PAUSED,
	DEAD0,
	DEAD1,
	DEAD2
};

class GameScreen : public Screen{
public:
	GameScreen(Game* game);
	void Start();
	void Update(float sec);
private:
	GameState state;
	Audio* music;
	Image* background;
	Image* ready_img;
	float onready_time;
	//apple stuff
	vector<Apple*> apples;
	float next_apple;
	void CalcApples(float sec);
	void SetApple();
	//ads
	float centerW;
	float centerH;
};