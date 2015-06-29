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
#include "MenuScreen.h"
#include "GameObject.h"
#include "Apple.h"
#include "Spider.h"
#include "Snake.h"
#include "Misc.h"
#include <Math.h>
#include <stdlib.h>

GameScreen::GameScreen(JimTheSnake* game):Screen(game){
	this->game = game;
}

void GameScreen::Start(){
	GameObject::Init(game);
	Snake::Init();
	Spider::Init(game);
	Apple::Init(game);
	snake = NULL;
	spider = NULL;
	//image loading
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50.f, game->GetHeight() + 50.f);
	ready_img = graphics->LoadImage("Game/ReadyTapLabel.png");
	score_img = graphics->LoadImage("Game/ScoreLabel.png");
	control_facepointer = graphics->LoadImage("Game/FacePointer.png");
	control_base = graphics->LoadImage("Game/ControlBase.png");
	pause_img = graphics->LoadImage("Game/PauseLabel.png");
	gameover_img = graphics->LoadImage("Game/GameOverLabel.png");
	Image* resumeup = graphics->LoadImage("Game/ResumeUp.png");
	Image* resumedown = graphics->LoadImage("Game/ResumeDown.png");
	Image* menuup = graphics->LoadImage("Game/MenuUp.png");
	Image* menudown = graphics->LoadImage("Game/MenuDown.png");
	Image* restartup = graphics->LoadImage("Game/RestartUp.png");
	Image* restartdown = graphics->LoadImage("Game/RestartDown.png");
	//positioning
	centerW = game->GetWidth() / 2;
	centerH = game->GetHeight() / 2;
	if(game->IsMusicEnabled()){
		music = new Audio("Game/GameMusic.mp3", true, true);
		game_over = new Audio("Game/GameOver.wav", false, true);
		music->Play();
	}else{
		music = NULL;
		game_over = NULL;
	}
	if(game->IsSoundEnabled()){
		punch = new Audio("Game/Punch.wav", false, false);
	}else{
		punch = NULL;
	}
	score_texter = new Texter(game, "NumbersRed.png", 60.f, 76.f, 10, 1, 48);
	snake = NULL;

	resume_btn = new Button(game, resumeup, resumedown);
	resume_btn->SetLocation(Point(450, centerH - 40));
	resume_btn->RegisterCallback(bind(&GameScreen::OnResumeClick, this));
	menu_btn = new Button(game, menuup, menudown);
	menu_btn->SetLocation(Point(450, centerH + 180));
	menu_btn->RegisterCallback(bind(&GameScreen::OnMenuClick, this));
	restart_btn = new Button(game, restartup, restartdown);
	restart_btn->SetLocation(Point(450, centerH - 40));
	restart_btn->RegisterCallback(bind(&GameScreen::OnRestartClick, this)); 
	state = GameState::ONREADY;
	Restart();
}

void GameScreen::Restart(){
	onready_time = 4.3f;
	game_over->Stop();
	music->Play();
	for(Apple* apple : apples){
		delete apple;
	}
	apples.clear();
	state = GameState::ONREADY;
	score_texter->SetScaleFactor(game->GetScaleFactor());
	graphics->ScaleImage(score_img, game->GetScaleFactor());
	delete snake;
	snake = new Snake();
	delete spider;
	spider = new Spider();
}

void GameScreen::Suspend(){
	state = GameState::PAUSED;
}

void GameScreen::Update(float sec){
	graphics->Clear(0.25, 0.25, 0);
	graphics->DrawImage(game->GetWidth()/2, game->GetHeight()/2, background);
	snake->Update(sec);
	/*
	DrawApples();
	spider->Draw();
	snake->DrawFace(sec);
	snake->DrawBody(sec);
	if(!spider->Eaten() && SpiderOnCollision()){
		bool dir = rand() % 2;
		if(dir){
			spider->Rotate(180.f);
		}else{
			spider->Rotate(-180.f);
		}
	}
	switch (state) {
	case GameState::ONREADY:{
		graphics->DrawImage(centerW, centerW, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			state = GameState::RUNNING;
		}
	}break;
	case GameState::RUNNING:{
			static bool down = false;
			snake->EatableNear(spider);
			spider->Update(sec, apples);
			CalcApples(sec);
			DrawScore();
			CalcInput(sec);
			snake->CalcHead(sec);
			//Snake die
			if(snake->OnBorder() || snake->OnBiteYouself()){
				graphics->ScaleImage(score_img, game->GetScaleFactor() * 1.2f);
				score_texter->SetScaleFactor(game->GetScaleFactor() * 1.2f);
				if(snake->GetScore() > game->BestScore()){
					game->SetBestScore(snake->GetScore());
				}
				state = GameState::DEAD0;
				time_dead01 = 1.f;
				music->Stop();
				punch->Play();
			}
			if(input->HaveKey() && input->GetKey() == Key::PAUSE){
				down = true;
			}
			if(down && !input->HaveKey()){
				state = GameState::PAUSED;
				music->Pause();
				down = false;
			}
		}break;
	case GameState::PAUSED:{
			static bool down = false;
			DrawScore();
			graphics->DrawImage(450, centerH - 250, pause_img);
			if(input->HaveKey() && input->GetKey() == Key::PAUSE){
				down = true;
			}
			if(down && !input->HaveKey()){
				state = GameState::RUNNING;
				music->Play();
				down = false;
			}
			resume_btn->Update();
			menu_btn->Update();
		}break;
	case GameState::DEAD0:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST0();
		time_dead01 -= sec;
		if(time_dead01 < 0) {
			state = GameState::DEAD1;
			time_dead02 = 1.5f;
			game_over->Play();
		}
		break;
	case GameState::DEAD1:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST1(sec);
		time_dead02 -= sec;
		if(time_dead02 < 0) {
			state = GameState::DEAD2;
		}
		break;
	case GameState::DEAD2:
		spider->Update(sec, apples);
		CalcApples(sec);
		snake->DrawFaceDeadST1(sec);
		graphics->DrawImage(450, centerH - 340, gameover_img);
		graphics->DrawImage(300, centerH - 210, score_img);
		score_texter->DrawText(520, centerH - 255, to_string(snake->GetScore()));
		restart_btn->Update();
		menu_btn->Update();
		break;
	default:
		break;
	}*/
}

void GameScreen::SetState(GameState newState){
	state = newState;
}

GameState GameScreen::GetState(){
	return state;
}

void GameScreen::CalcApples(float sec){
	static float next_apple = 0; 
	if(next_apple <= 0) {
		next_apple = (float)(rand()%15);
		SetApple();
	} else {
		next_apple -= sec;
	}
	if(apples.size() == 0)
		SetApple();
	auto it = apples.begin();
	while(it != apples.end()){
		if(!(*it)->Eaten()){
			snake->EatableNear(*it);
			(*it)->Update(sec);
			it++;
		} else {
			delete *it;
			*it = NULL;
			it = apples.erase(it);
			int roll = rand() % 101;
			if(roll > 20){
				spider->Start();
			}
		}
	}
}

void GameScreen::SetApple(){
	Apple* apple = new Apple();;
	float top = score_img->GetHeight() + 50;
	float bottom = game->GetHeight() - 4 * apple->GetRadius();
	float left = 4 * apple->GetRadius();
	float right = game->GetWidth() - 4 * apple->GetRadius();
	Point apple_pos;
	bool onSnake = true;
	while(onSnake) {
		onSnake = false;
		int randX = (int)(right - left);
		int randY = (int)(bottom - top);
		apple_pos.x = (float)(rand() % randX) + left;
		apple_pos.y = (float)(rand() % randY) + top;
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
	static const Point pos(game->GetWidth() / 2 + 120, 50);
	graphics->DrawImage(pos, score_img);
	float offset = score_texter->GetWidth() / 2;
	if(snake->GetScore() > 9)
		offset = score_texter->GetWidth();
	if(snake->GetScore() > 99)
		offset = score_texter->GetWidth() * 2;
	score_texter->DrawText(game->GetWidth() / 2 + 380 - offset, 20, to_string(snake->GetScore()));
}

void GameScreen::CalcInput(float sec){
	static Point control_pos;
	if(input->HaveInput()){
		if(control_pos.x == 0 && control_pos.y == 0){
			control_pos.x = input->GetInput().x;
			control_pos.y = input->GetInput().y;
			return;
		}
		float fingerAngle = Angle(control_pos, input->GetInput());
		//determine in witch direction we need to move
		float clockwise;
		if(fingerAngle > snake->Direction())
			clockwise = 360 + snake->Direction() - fingerAngle;
		else 
			clockwise = snake->Direction() - fingerAngle;
		float counterclockwise = 360 - clockwise;
		//rotate snake head
		if(clockwise < snake->GetSpeedW() * sec || counterclockwise < snake->GetSpeedW() * sec) {
			snake->Rotate(fingerAngle);
		} else {
			if(counterclockwise < clockwise) 
				snake->Rotate(snake->Direction() + snake->GetSpeedW() * sec);
			else
				snake->Rotate(snake->Direction() - snake->GetSpeedW() * sec);
			if(snake->Direction() >= 180.f)
				snake->Rotate(snake->Direction() - 360.f);
			if(snake->Direction() <= -180.f)
				snake->Rotate(snake->Direction() + 360.f);
		}
		//draw control elements
		graphics->Rotate(control_facepointer, snake->Direction());
		graphics->DrawImage(control_pos, control_base);
		graphics->DrawImage(control_pos.x, control_pos.y - 2, control_facepointer);
	}else{
		control_pos.x = 0;
		control_pos.y = 0;
	}
}

bool GameScreen::SpiderOnCollision(){
	Point spiderAhead(spider->GetPosition().x, spider->GetPosition().y);
	spiderAhead.y += -spider->GetSpeedV() * sin(spider->GetAngle() / 180.0f * PI) * 0.1;
	spiderAhead.x += spider->GetSpeedV() * cos(spider->GetAngle() / 180.0f * PI) * 0.1;
	return snake->OnCollision(spiderAhead, spider->GetRadius());
}

GameScreen::~GameScreen(){
	delete snake;
	delete spider;
	Snake::Release();
	Spider::Release();
	Apple::Release();

	delete music;
	delete game_over;
	delete punch;
	for(Apple* apple : apples){
		delete apple;
	}
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(score_img);
	graphics->ReleaseImage(gameover_img);
	graphics->ReleaseImage(control_base);
	graphics->ReleaseImage(control_facepointer);
	graphics->ReleaseImage(pause_img);
	graphics->ReleaseImage(ready_img);

	delete resume_btn;
	delete restart_btn;
	delete menu_btn;

	delete score_texter;
}

void GameScreen::OnMenuClick(){
	game->SetScreen(new MenuScreen(game));
}

void GameScreen::OnResumeClick(){
	state = GameState::RUNNING;
}

void GameScreen::OnRestartClick(){
	Restart();
}
