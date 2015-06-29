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

class GameObject;
class Snake;
class Spider;
class Apple;
class Cactus;
class GameScreen;

enum GameState{
	RUNNING,
	ONREADY,
	PAUSED,
	GAMEOVER
};

enum GameEvent{
	STATE_CHANGED
};

class GameListener{
public:
	virtual void GameEventAccured(GameScreen* sender, GameEvent e);
};

class GameScreen : public Screen{
public:
	GameScreen(JimTheSnake* game);
	void Start();
	void Suspend();
	void Update(float sec);
	void AddGameObject(GameObject* gameObject);
	void RegisterListener(GameListener* listener);
	GameState GetGameState();
	void IncreaseScore(int gain);
	~GameScreen();
private:
	JimTheSnake* game;
	GameState state;
	list<GameObject*> game_objects;
	list<GameListener*> listeners;
	Snake* snake;
	int apple_count;
	int score;
	float onready_time;

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

	float centerW;
	float centerH;
	//float time_dead01;
	//float time_dead02;

	void Restart();
	void CalcInput(float sec);

	//void DrawApples();
	void DrawScore();

	void CalcApples(float sec);
	void SetApple();
	//bool SpiderOnCollision();

	void UpdateObjects(float sec);
	void OnStateChanged();

	void OnResumeClick();
	void OnMenuClick();
	void OnRestartClick();
};
