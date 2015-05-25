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
#include "Texter.h"
#include <list>

class Snake;
class Spider;
class Apple;

enum class GameState{
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
	void AddScore(int gain);
	void StartSpider();
	~GameScreen();
private:
	GameState state;
	Snake* snake;
	Audio* music;
	Image* background;
	bool music_enable;
	bool sound_enable;
	Image* ready_img;
	float onready_time;
	int score;
	Texter* score_texter;
	Image* score_img;

	//apple stuff
	list<Apple*> apples;
	float next_apple;
	void Restart();
	void DrawApples();
	void DrawScore();
	void CalcApples(float sec);
	void SetApple();
	//control stuff
	Image* control_base;
	Image* control_facepointer;
	PointX control_pos;
	void CalcInput(float delta);
	//ads
	float centerW;
	float centerH;
	Spider* spider;
};