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
#include "JimTheSnake.h"
#include "Screen.h"
#include "Texter.h"
#include "Button.h"

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
	GameScreen(JimTheSnake* game);
	void Start();
	void Update(float sec);
	~GameScreen();
private:
	JimTheSnake* game;
	GameState state;
	Snake* snake;
	Spider* spider;
	list<Apple*> apples;

	Image* background;
	Image* score_img;
	Image* gameover_img;
	Image* control_base;
	Image* control_facepointer;
	Image* pause_img;
	Image* ready_img;

	Audio* music;
	Audio* punch;
	Audio* game_over;

	Button* resume_btn;
	Button* menu_btn;
	Button* restart_btn;

	Texter* score_texter;

	bool music_enable;
	bool sound_enable;
	float onready_time;
	float centerW;
	float centerH;
	float time_dead01;
	float time_dead02;

	void Restart();
	void DrawApples();
	void DrawScore();
	void CalcApples(float sec);
	void SetApple();
	bool SpiderOnCollision();
	void CalcInput(float delta);

	void OnResumeClick();
	void OnMenuClick();
	void OnRestartClick();
};
