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
	
#include "GameScreen.h"
#include "Apple.h"
#include "Spider.h"
#include "Snake.h"

GameScreen::GameScreen(Game* game):Screen(game){ }

void GameScreen::Start(){
	state = GameState::ONREADY;
	centerW = game->GetWidth() / 2;
	centerH = game->GetHeight() / 2;
	onready_time = 4.3f;
	music = new Audio("Game/GameMusic.mp3", true, true);
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50.f, game->GetHeight() + 50.f);
	ready_img = graphics->LoadImage("Game/ReadyTapLabel.png");
	Apple::Init(game);
	Spider::Init(game);

	music->Play();
}

void GameScreen::Update(float sec){
	graphics->Clear(1,0,0);
	graphics->DrawImage(game->GetWidth() /2, game->GetHeight()/2, background);

	switch (state)
	{
	case GameState::RUNNING:
		break;
	case GameState::ONREADY:
		graphics->DrawImage(centerW, centerW, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			state = GameState::RUNNING;
		}
		break;
	case GameState::PAUSED:
		break;
	case GameState::DEAD0:
		break;
	case GameState::DEAD1:
		break;
	case GameState::DEAD2:
		break;
	default:
		break;
	}
}

void GameScreen::CalcApples(float sec){
	if(next_apple <= 0) {
		next_apple = rand()%15;
		SetApple();
	} else {
		next_apple -= sec;
	}
	if(apples.size() == 0)
		SetApple();
		/*
	for(int i = 0; i < apples.size(); i++) {
		Apple* apple = apples[i];
		if(apple->GetLifeTime() > 0) {
			snake.OnNear(apple);
			apple->Update(sec);
			//apple.Draw();
		} else {
			apples.remove(i);
			apples.erase(
			i--;
		}
	}*/

	for(auto it = apples.begin(); it != apples.end(); it++){
		if((*it)->GetLifeTime() > 0){
			snake->EatableNear(*it);
			(*it)->Update(sec);
		} else {
			apples.erase(it, it);
		}
	}
}

void GameScreen::SetApple(){
	Apple* apple = new Apple();;
	//float top = score_img.GetHeight() + 50;
	float top = 50;
	float bottom = game->GetHeight() - 4 * apple->GetRadius();
	float left = 4 * apple->GetRadius();
	float right = game->GetWidth() - 4 * apple->GetRadius();
	PointX apple_pos;
	boolean onSnake = true;
	while(onSnake) {
		onSnake = false;
		int randX = ((int)(right - left)) + left;
		int randY = ((int)(bottom - top)) + top;
		apple_pos.x = rand() % randX;
		apple_pos.y = rand() % randY;
		onSnake = snake->OnCollision(apple_pos, apple->GetRadius());
	}
	apple->SetPosition(apple_pos);
	apples.push_back(apple);
}