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
	snake = new Snake(game);
	spider = new Spider();
	music_enable = saver->LoadBool(KEY_MUSIC);
	sound_enable = saver->LoadBool(KEY_SOUND);
	if(music_enable){
		music->Play();
	}
	score_texter = ((SnakyGame*)game)->score_texter;
	score_img = graphics->LoadImage("Game/ScoreLabel.png");
	Restart();
}

void GameScreen::Restart(){
	score = 0;
	score_texter->SetScaleFactor(game->GetScaleFactor());
	graphics->ScaleImage(score_img, game->GetScaleFactor());
}

void GameScreen::Update(float sec){
	graphics->Clear(0.25, 0.25, 0);
	graphics->DrawImage(game->GetWidth() /2, game->GetHeight()/2, background);
	DrawScore();
	DrawApples();
	spider->Draw();
	switch (state)
	{
	case GameState::RUNNING:
		spider->Update(sec, apples);
		CalcApples(sec);
		break;
	case GameState::ONREADY:
		graphics->DrawImage(centerW, centerW, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			state = GameState::RUNNING;
			spider->Start();
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
	
	graphics->DrawCircle(PointX(centerW, centerH), 2, ColorX::Red);
	graphics->DrawLine(PointX(centerW, 0), PointX(centerW, game->GetHeight()), ColorX::Red);
	graphics->DrawLine(PointX(0, centerH), PointX(game->GetWidth(), centerH), ColorX::Red);
	if(input->HaveInput()){
		graphics->DrawCircle(input->GetInput(), 2, ColorX::Green);
		graphics->DrawLine(PointX(centerW, centerH), input->GetInput(), ColorX::Green);
		launcher->LogIt("Angle - " + to_string(Angle(PointX(centerW, centerH), input->GetInput())));
	}
}

void GameScreen::CalcApples(float sec){
	if(next_apple <= 0) {
		next_apple = (float)(rand()%15);
		SetApple();
	} else {
		next_apple -= sec;
	}
	if(apples.size() == 0)
		SetApple();

	for(auto it = apples.begin(); it != apples.end(); it++){
		if((*it)->GetLifeTime() > 0){
			snake->EatableNear(*it);
			(*it)->Update(sec);
		} else {
			delete *it;
			apples.erase(it++);
		}
	}
}

void GameScreen::SetApple(){
	Apple* apple = new Apple();;
	float top = score_img->GetHeight() + 50;
	float bottom = game->GetHeight() - 4 * apple->GetRadius();
	float left = 4 * apple->GetRadius();
	float right = game->GetWidth() - 4 * apple->GetRadius();
	PointX apple_pos;
	bool onSnake = true;
	while(onSnake) {
		onSnake = false;
		int randX = (int)((right - left) + left);
		int randY = (int)((bottom - top) + top);
		apple_pos.x = (float)(rand() % randX);
		apple_pos.y = (float)(rand() % randY);
		onSnake = snake->OnCollision(apple_pos, apple->GetRadius());
	}
	apple->SetPosition(apple_pos);
	apples.push_back(apple);
}

void GameScreen::DrawApples(){
	for(Apple* apple : apples) {
		apple->Draw();
	}
}

void GameScreen::DrawScore(){
	static const PointX pos(game->GetWidth() / 2 + 120, 50);
	graphics->DrawImage(pos, score_img);
	float offset = score_texter->GetWidth() / 2;
	if(score > 9)
		offset = score_texter->GetWidth();
	if(score > 99)
		offset = score_texter->GetWidth() * 2;
	score_texter->DrawText(game->GetWidth() / 2 + 380 - offset, 20, to_string(score));
}

void GameScreen::AddScore(int gain){
	score += gain;
}

void GameScreen::StartSpider(){
	spider->Start();
}

GameScreen::~GameScreen(){
	delete snake;
	delete music;
	for(Apple* apple : apples){
		delete apple;
	}
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(ready_img);
	Spider::Release();
}