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
class Cactus;

enum class GameState{
	ONREADY,
	RUNNING,
	PAUSED,
	GAMEOVER,
};

class GameScreen : public Screen{
public:
	GameScreen(JimTheSnake* game);
	~GameScreen();

	void Start();
	void Update(float sec);
	void Suspend();

	GameState GetState();
	void SetState(GameState newState);
	int GetScore();
	void SetScore(int score);
	list<Apple*>& GetApples();
	list<Spider*>& GetSpiders();
	list<Cactus*>& GetCactuses();
	void MusicStop();
private:
	JimTheSnake* game;
	GameState state;
	Snake* snake;
	list<Apple*> apples;
	list<Spider*> spiders;
	list<Cactus*> cactuses;

	Image* background;
	Image* score_img;
	Image* gameover_img;
	Image* control_base;
	Image* control_facepointer;
	Image* pause_img;
	Image* ready_img;

	Audio* music;
	Audio* game_over;

	Button* resume_btn;
	Button* menu_btn;
	Button* restart_btn;

	Texter* score_texter;

	bool music_enable;
	bool sound_enable;
	int score;
	float onready_time;
	float centerW;
	float centerH;

	void Restart();
	void UpdateApples(float sec);
	void DrawApples();
	void UpdateSpiders(float sec);
	void DrawSpiders();
	void UpdateCactuses(float sec);
	void DrawCactuses();
	void DrawScore();
	void CalcInput(float sec);
	Point GetEmptyPosition(float radius);
	//bool SpiderOnCollision();

	void OnResumeClick();
	void OnMenuClick();
	void OnRestartClick();
};
