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
#include "Cactus.h"
#include "Apple.h"
#include "Spider.h"
#include "Snake.h"
#include "Misc.h"
#include <Math.h>
#include <stdlib.h>
//						CONSTRUCTOR
GameScreen::GameScreen(JimTheSnake* game):Screen(game){
	this->game = game;
}
//						DESTRUCTOR
GameScreen::~GameScreen(){
	for(Apple* apple : apples){
		delete apple;
	}
	apples.clear();
	for(Spider* spider : spiders){
		delete spider;
	}
	spiders.clear();
	for(Cactus* cactus : cactuses){
		delete cactus;
	}
	delete snake;
	cactuses.clear();

	graphics->ReleaseImage(background);
	graphics->ReleaseImage(score_img);
	graphics->ReleaseImage(score_bright_img);
	graphics->ReleaseImage(gameover_img);
	graphics->ReleaseImage(control_base);
	graphics->ReleaseImage(control_facepointer);
	graphics->ReleaseImage(pause_img);
	graphics->ReleaseImage(ready_img);

	delete back_btn;
	delete restart_btn;
	delete menu_btn;
	delete score_texter;

	delete music;
	delete game_over;
	delete apple_drop;

	Snake::Release();
	Spider::Release();
	Apple::Release();
	Cactus::Release();
}
//						OVERRIDDEN METHODS
void GameScreen::Start(){
	srand(time(0));
	GameObject::Init(game);
	Snake::Init();
	Apple::Init();
	Spider::Init();
	Cactus::Init();
	snake = NULL;
	music = NULL;
	commercial = NULL;
	game_over = NULL;
	apple_drop = NULL;
	//image loading
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50.f, game->GetHeight() + 50.f);
	ready_img = graphics->LoadImage("Game/ReadyLabel.png");
	score_img = graphics->LoadImage("Game/ScoreLabel.png");
	score_bright_img = graphics->LoadImage("Game/ScoreBrightLabel.png");
	control_facepointer = graphics->LoadImage("Game/FacePointer.png");
	control_base = graphics->LoadImage("Game/ControlBase.png");
	pause_img = graphics->LoadImage("Game/PauseLabel.png");
	gameover_img = graphics->LoadImage("Game/GameOverLabel.png");
	Image* backup = graphics->LoadImage("Game/BackUp.png");
	Image* backdown = graphics->LoadImage("Game/BackDown.png");
	Image* menuup = graphics->LoadImage("Game/MenuUp.png");
	Image* menudown = graphics->LoadImage("Game/MenuDown.png");
	Image* restartup = graphics->LoadImage("Game/RestartUp.png");
	Image* restartdown = graphics->LoadImage("Game/RestartDown.png");
	Image* pause = graphics->LoadImage("Game/PauseButton.png");
	//positioning
	centerW = game->GetWidth() / 2;
	centerH = game->GetHeight() / 2;
	if(game->IsMusicEnabled()){
		music = new Audio("Game/GameMusic.mp3", true, true);
		game_over = new Audio("Game/GameOver.mp3", false, true);
	}
	if(game->IsSoundEnabled()){
		apple_drop = new Audio("Game/AppleDrop.wav", false, false);
	}
	score_texter = new Texter(game, "NumbersRed.png", 60.f, 76.f, 10, 1, 48);

	pause_btn = new Button(game, pause, NULL);
	pause_btn->SetLocation(Point(pause_btn->GetWidth()/3*2, pause_btn->GetHeight()/3*2));
	pause_btn->RegisterCallback(bind(&GameScreen::OnPauseClick, this));
	back_btn = new Button(game, backup, backdown);
	back_btn->SetLocation(Point(450, centerH - 40));
	back_btn->RegisterCallback(bind(&GameScreen::OnResumeClick, this));
	menu_btn = new Button(game, menuup, menudown);
	menu_btn->SetLocation(Point(450, centerH + 180));
	menu_btn->RegisterCallback(bind(&GameScreen::OnMenuClick, this));
	restart_btn = new Button(game, restartup, restartdown);
	restart_btn->SetLocation(Point(450, centerH - 40));
	restart_btn->RegisterCallback(bind(&GameScreen::OnRestartClick, this)); 
	Restart();
}

void GameScreen::Update(float sec){
	graphics->Clear(0.25, 0.25, 0);
	graphics->DrawImage(game->GetWidth()/2, game->GetHeight()/2, background);
	ProccessCollisions();
	UpdateApples(sec);
	DrawApples();
	UpdateCactuses(sec);
	DrawCactuses();
	UpdateSpiders(sec);
	DrawSpiders();
	snake->Update(sec);
	snake->Draw();
	switch (state) {
	case GameState::ONREADY:
		graphics->DrawImage(centerW, game->GetHeight() / 3, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			//SetState(GameState::RUNNING);
			state = GameState::RUNNING;
		}
		break;
	case GameState::RUNNING:{
		static bool down = false;
		CalcInput(sec);
		DrawScore();
		if(input->HaveKey() && input->GetKey() == Key::PAUSE){
			down = true;
		}
		if(down && !input->HaveKey()){
			SetState(GameState::PAUSED);
			down = false;
		}
		pause_btn->Update();
		}break;
	case GameState::PAUSED:{
			static bool down = false;
			DrawScore();
			graphics->DrawImage(450, centerH - 250, pause_img);
			if(input->HaveKey() && input->GetKey() == Key::PAUSE){
				down = true;
			}
			if(down && !input->HaveKey()){
				SetState(GameState::RUNNING);
				down = false;
			}
			back_btn->Update();
			menu_btn->Update();
		}break;
	case GameState::GAMEOVER:
		graphics->DrawImage(450, centerH - 340, gameover_img);
		graphics->DrawImage(300, centerH - 210, score_bright_img);
		score_texter->DrawText(520, centerH - 255, to_string(score));
		restart_btn->Update();
		menu_btn->Update();
		break;
	default:
		break;
	}
}

void GameScreen::Suspend(){
	if(state != GameState::GAMEOVER && state != GameState::PAUSED){
		SetState(GameState::PAUSED);
	}
}
//						PUBLIC METHODS	
GameState GameScreen::GetState(){
	return state;
}

void GameScreen::SetState(GameState newState){
	switch (newState){
	case GameState::ONREADY:
		break;
	case GameState::RUNNING:
		music->Resume();
		for(Spider* spider : spiders){
			spider->Resume();
		}
		break;
	case GameState::PAUSED:
		music->Pause();
		for(Spider* spider : spiders){
			spider->Pause();
		}
		break;
	case GameState::GAMEOVER:
		game_over->Play();
		if(score > game->BestScore()){
			game->SetBestScore(score);
		}
		break;
	default:
		throw string("Unexpected GameState");
	}
	state = newState;
}

void GameScreen::AddScore(int gain){
	score += gain;
}

void GameScreen::StopMusic(){
	music->Stop();
}

//						PRIVATE METHODS	
void GameScreen::Restart(){
	score = 0;
	onready_time = 4.3f;
	game_over->Stop();
	music->Play();
	score_texter->SetScaleFactor(game->GetScaleFactor());
	//graphics->ScaleImage(score_img, game->GetScaleFactor());
	delete snake;
	snake = NULL;
	for(Apple* apple : apples){
		delete apple;
	}
	apples.clear();
	for(Spider* spider : spiders){
		delete spider;
	}
	spiders.clear();
	for(Cactus* cactus : cactuses){
		delete cactus;
	}
	cactuses.clear();
	snake = new Snake();
	SetState(GameState::ONREADY);
	for(int i = 0; i < 2; i++){
		Cactus* newOne = new Cactus();
		newOne->SetPosition(GetEmptyPosition(newOne->GetRadius()));
		cactuses.push_back(newOne);
	}
	//debug
	/*
	for(int i = 0; i < 1; i++){
		Apple* newApple1 = new Apple();
		newApple1->SetPosition(GetEmptyPosition(newApple1->GetRadius()));
		apples.push_back(newApple1);
		Apple* newApple2 = new Apple();
		newApple2->SetPosition(GetEmptyPosition(newApple2->GetRadius()));
		apples.push_back(newApple2);
		spiders.push_back(new Spider());
		Cactus* newOne = new Cactus();
		newOne->SetPosition(GetEmptyPosition(newOne->GetRadius()));
		cactuses.push_back(newOne);
	}*/
}

void GameScreen::ProccessCollisions(){
	Collisioner* snakeRadar = snake->GetRadar();
	vector<Body*> snakeBody = snake->GetBodyNodes();
	Collisioner* snakeHead = snake;
	for(Apple* apple : apples){
		if(CircleOnCollision(snakeRadar->GetPosition(), snakeRadar->GetRadius(), apple->GetPosition(), apple->GetRadius())){
			snakeRadar->CheckCollision(apple);
			apple->CheckCollision(snakeRadar);
		}
	}
	for(Cactus* cactus : cactuses){
		if(CircleOnCollision(cactus->GetPosition(), cactus->GetRadius(), snakeHead->GetPosition(), snakeHead->GetRadius())){
			cactus->CheckCollision(snakeHead);
			snakeHead->CheckCollision(cactus);
		}
	}
	for(Body* b : snakeBody){
		if(CircleOnCollision(b->GetPosition(), b->GetRadius(), snakeHead->GetPosition(), snakeHead->GetRadius())){
			b->CheckCollision(snakeHead);
			snakeHead->CheckCollision(b);
		}
	}

	for(Spider* spider : spiders){
		vector<Collisioner*> radars = spider->GetRadars();
		if(CircleOnCollision(snakeRadar->GetPosition(), snakeRadar->GetRadius(), spider->GetPosition(), spider->GetRadius())){
			snakeRadar->CheckCollision(spider);
			spider->CheckCollision(snakeRadar);
		}
		for(Apple* apple : apples){
			if(CircleOnCollision(apple->GetPosition(), apple->GetRadius(), spider->GetPosition(), spider->GetRadius())){
				spider->CheckCollision(apple);
				apple->CheckCollision(spider);
			}
		}
		for(Collisioner* spiderRadar : radars){
			for(Body* b : snakeBody){
				if(CircleOnCollision(b->GetPosition(), b->GetRadius(), spiderRadar->GetPosition(), spiderRadar->GetRadius())){
					spiderRadar->CheckCollision(b);
					b->CheckCollision(spiderRadar);
				}
			}
			for(Cactus* cactus : cactuses){
				if(CircleOnCollision(cactus->GetPosition(), cactus->GetRadius(), spiderRadar->GetPosition(), spiderRadar->GetRadius())){
					cactus->CheckCollision(spiderRadar);
					spiderRadar->CheckCollision(cactus);
				}
			}
			if(CircleOnCollision(snakeHead->GetPosition(), snakeHead->GetRadius(), spiderRadar->GetPosition(), spiderRadar->GetRadius())){
				snakeHead->CheckCollision(spiderRadar);
				spiderRadar->CheckCollision(snakeHead);
			}
		}
	}
}

void GameScreen::UpdateApples(float sec){
	if(GetState() != GameState::PAUSED){
		static float next = 0; 
		auto it = apples.begin();
		while(it != apples.end()){
			Apple* apple = (*it);
			if(!apple->Eaten()){
				apple->Update(sec);
				it++;
			}else{
				delete apple;
				it = apples.erase(it);
				int roll = rand() % 101;
				if(roll > 75) {
					spiders.push_back(new Spider());
				}
			}
		}

		if(next <= 0 || apples.size() == 0){
			next = (float)(rand()%15);
			Apple* newApple = new Apple();
			newApple->SetPosition(GetEmptyPosition(newApple->GetRadius()));
			apples.push_back(newApple);
			apple_drop->Play();
		}else{
			next -= sec;
		}
	}
}

void GameScreen::DrawApples(){
	for(Apple* apple : apples){
		apple->Draw();
	}
}

void GameScreen::UpdateSpiders(float sec){
	if(GetState() != GameState::PAUSED){
		auto it = spiders.begin();
		while(it != spiders.end()){
			Spider* spider = (*it);
			if(!spider->Eaten()){
				spider->Update(sec);
				it++;
			}else{
				delete spider;
				it = spiders.erase(it);
			}
		}
	}
}

void GameScreen::DrawSpiders(){
	for(Spider* spider : spiders){
		spider->Draw();
	}
}

void GameScreen::UpdateCactuses(float sec){
	if(GetState() != GameState::PAUSED){
		static float next = 0;
		auto it = cactuses.begin();
		while(it != cactuses.end()){
			Cactus* cactus = (*it);
			if(!cactus->IsDead()){
				cactus->Update(sec);
				++it;
			}else{
				delete cactus;
				it = cactuses.erase(it);
			}
		}

		if(next < 0){
			next = (float)(rand()%15);
			Cactus* newOne = new Cactus();
			newOne->SetPosition(GetEmptyPosition(newOne->GetRadius()));
			cactuses.push_back(newOne);
		}else{
			next -= sec;
		}
	}
}

void GameScreen::DrawCactuses(){
	for(Cactus* cactus : cactuses){
		cactus->Draw();
	}
}

void GameScreen::DrawScore(){
	static const Point pos(game->GetWidth() / 2 + 120, 50);
	graphics->DrawImage(pos, score_img);
	float offset = score_texter->GetWidth() / 2;
	if(score > 9)
		offset = score_texter->GetWidth();
	if(score > 99)
		offset = score_texter->GetWidth() * 2;
	score_texter->DrawText(game->GetWidth() / 2 + 380 - offset, 20, to_string(score));
}

void GameScreen::CalcInput(float sec){
	static Point control_pos;
	if(input->HaveInput()){
		if(control_pos.x == 0 && control_pos.y == 0){
			control_pos.x = input->GetInput().x;
			control_pos.y = input->GetInput().y;
			return;
		}
		//test
		float NeedDistance = 300.f;
		float RealDistance = Distance(control_pos, input->GetInput());
		if(RealDistance > NeedDistance){
			float fingerAngle = Angle(control_pos, input->GetInput());
			float deltaDist = RealDistance - NeedDistance;
			control_pos.x += deltaDist * cos(fingerAngle / 180.f * PI);
			control_pos.y += -deltaDist * sin(fingerAngle / 180.f * PI);
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
		//graphics->Rotate(control_facepointer, snake->Direction());
		//graphics->DrawImage(control_pos, control_base);
		//graphics->DrawImage(control_pos.x, control_pos.y - 2, control_facepointer);
	}else{
		control_pos.x = 0;
		control_pos.y = 0;
	}
}

bool GameScreen::IsEmptyPosition(Point pos, float radius){
	if(CircleOnCollision(pos, radius, snake->GetPosition(), snake->GetRadius()))
		return false;
	for(GameObject* body : snake->GetBodyNodes()){
		if(CircleOnCollision(pos, radius, body->GetPosition(), body->GetRadius()))
			return false;
	}
	for(Apple* apple : apples){
		if(CircleOnCollision(pos, radius, apple->GetPosition(), apple->GetRadius()))
			return false;
	}
	for(Spider* spider : spiders){
		if(CircleOnCollision(pos, radius, spider->GetPosition(), spider->GetRadius()))
			return false;
	}
	for(Cactus* cactus : cactuses){
		if(CircleOnCollision(pos, radius, cactus->GetPosition(), cactus->GetRadius())){
			return false;
		}
	}
	return true;
}

list<Apple*>& GameScreen::GetApples(){
	return apples;
}

Point GameScreen::GetEmptyPosition(float radius){
	float top = score_img->GetHeight() + 50;
	float bottom = game->GetHeight() - 4 * radius;
	float left = 4 * radius;
	float right = game->GetWidth() - 4 * radius;
	Point position;
	bool onCollision = true;
	while(onCollision) {
		onCollision = false;
		int randX = (int)(right - left);
		int randY = (int)(bottom - top);
		position.x = (float)(rand() % randX) + left;
		position.y = (float)(rand() % randY) + top;
		onCollision = !IsEmptyPosition(position, radius);
	}
	return position;
}

void GameScreen::OnMenuClick(){
	game_over->Stop();
	game->SetScreen(new MenuScreen(game));
}

void GameScreen::OnResumeClick(){
	SetState(GameState::RUNNING);
}

void GameScreen::OnRestartClick(){
	Restart();
}

void GameScreen::OnPauseClick(){
	SetState(GameState::PAUSED);
	launcher->LogIt("There");
}
