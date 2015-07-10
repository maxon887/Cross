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
	delete snake;
	Snake::Release();
	Spider::Release();
	Apple::Release();
	Cactus::Release();/*
	for(Apple* apple : apples){
		delete apple;
	}
	for(Spider* spider : spiders){
		delete spider;
	}
	for(Cactus* cactus : cactuses){
		delete cactus;
	}*/
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
	delete music;
	delete game_over;
	delete score_texter;
}
//						OVERRIDDEN METHODS
void GameScreen::Start(){
	GameObject::Init(game);
	Snake::Init();
	Apple::Init();
	Spider::Init();
	Cactus::Init();
	snake = NULL;
	music = NULL;
	game_over = NULL;
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
	}
	score_texter = new Texter(game, "NumbersRed.png", 60.f, 76.f, 10, 1, 48);

	resume_btn = new Button(game, resumeup, resumedown);
	resume_btn->SetLocation(Point(450, centerH - 40));
	resume_btn->RegisterCallback(bind(&GameScreen::OnResumeClick, this));
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
	graphics->DrawImage(game->GetWidth()/2, game->GetHeight()/2, background);/*
	UpdateApples(sec);
	DrawApples();
	UpdateCactuses(sec);
	DrawCactuses();
	UpdateSpiders(sec);
	DrawSpiders();*/
	//snake->Update(sec);
	//snake->Draw();
	for(pair<int, GameObject*> pair : objects){
		GameObject* obj = pair.second;
		obj->Update(sec);
		obj->Draw();
	}
	switch (state) {
	case GameState::ONREADY:
		graphics->DrawImage(centerW, centerW, ready_img);
		onready_time -= sec;
		if(onready_time < 0 || input->HaveInput()){
			SetState(GameState::RUNNING);
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
	case GameState::GAMEOVER:
		graphics->DrawImage(450, centerH - 340, gameover_img);
		graphics->DrawImage(300, centerH - 210, score_img);
		score_texter->DrawText(520, centerH - 255, to_string(GetScore()));
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
		break;
	case GameState::PAUSED:
		music->Pause();
		break;
	case GameState::GAMEOVER:
		break;
	default:
		break;
	}
	state = newState;
}

int GameScreen::GetScore(){
	return score;
}

void GameScreen::SetScore(int score){
	this->score = score;
}

void GameScreen::AddObject(GameObject* obj){
	objects.insert(pair<int, GameObject*>(obj->GetId(), obj));
}

void GameScreen::DeleteObject(GameObject* obj){
	auto it = objects.find(obj->GetId());
	objects.erase(it);
}
/*
Snake* GameScreen::GetSnake(){
	return snake;
}*/
/*
list<Apple*>& GameScreen::GetApples(){
	return apples;
}

list<Spider*>& GameScreen::GetSpiders(){
	return spiders;
}

list<Cactus*>& GameScreen::GetCactuses(){
	return cactuses;
}
*/
void GameScreen::MusicStop(){
	music->Stop();
}
//						PRIVATE METHODS	
void GameScreen::Restart(){
	score = 0;
	onready_time = 4.3f;
	game_over->Stop();
	music->Play();
	//apples.clear();
	//spiders.clear();
	//cactuses.clear();
	score_texter->SetScaleFactor(game->GetScaleFactor());
	graphics->ScaleImage(score_img, game->GetScaleFactor());
	delete snake;
	/*
	for(Apple* apple : apples){
		delete apple;
	}
	for(Spider* spider : spiders){
		delete spider;
	}
	for(Cactus* cactus : cactuses){
		delete cactus;
	}*/
	snake = new Snake();
	AddObject(snake);
	//objects.insert(snake);
	/*
	for(int i = 0; i < 2; i++){
		Cactus* cactus = new Cactus();
		cactus->SetPosition(GetEmptyPosition(cactus->GetRadius()));
		cactuses.push_back(cactus);

		spiders.push_back(new Spider());
	}*/
	SetState(GameState::ONREADY);
}

void GameScreen::SetApple(){

}
/*
void GameScreen::UpdateApples(float sec){
	if(GetState() != GameState::PAUSED){
		static float next = 0; 
		auto it = apples.begin();
		while(it != apples.end()) {
			Apple* apple = (*it);
			if(!apple->Eaten()) {
				apple->Update(sec);
				it++;
			} else {
				delete apple;
				it = apples.erase(it);
				int roll = rand() % 101;
				if(roll > 20) {
					spiders.push_back(new Spider());
				}
			}
		}
		if(next <= 0 || apples.size() == 0) {
			next = (float)(rand()%15);
			Apple* newApple = new Apple();
			newApple->SetPosition(GetEmptyPosition(newApple->GetRadius()));
			apples.push_back(newApple);
		} else {
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
			if(!spider->Eaten() || !spider->Hiding()){
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
		static float next = (float)(rand()%15);
		auto it = cactuses.begin();
		while(it != cactuses.end()){
			Cactus* cactus = (*it);
			if(!cactus->IsDead()){
				cactus->Update(sec);
				it++;
			}else{
				delete cactus;
				it = cactuses.erase(it);
			}
		}
		if(next <= 0) {
			next = (float)(rand()%15);
			Cactus* cactus = new Cactus();
			cactus->SetPosition(GetEmptyPosition(cactus->GetRadius()));
			cactuses.push_back(cactus);
		} else {
			next -= sec;
		}
	}
}

void GameScreen::DrawCactuses(){
	for(Cactus* cactus : cactuses){
		cactus->Draw();
	}
}*/

void GameScreen::DrawScore(){
	static const Point pos(game->GetWidth() / 2 + 120, 50);
	graphics->DrawImage(pos, score_img);
	float offset = score_texter->GetWidth() / 2;
	if(GetScore() > 9)
		offset = score_texter->GetWidth();
	if(GetScore() > 99)
		offset = score_texter->GetWidth() * 2;
	score_texter->DrawText(game->GetWidth() / 2 + 380 - offset, 20, to_string(GetScore()));
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
		for(pair<int, GameObject*> pair : objects){
			GameObject* obj = pair.second;
			onCollision = CircleOnCollision(position, radius, obj->GetPosition(), obj->GetRadius());
			if(onCollision)
				break;
		}
		/*
		onCollision = snake->OnCollision(position, radius);
		if(onCollision)
			continue;
		for(Apple* apple : apples){
			onCollision = CircleOnCollision(position, radius, apple->GetPosition(), apple->GetRadius());
			if(onCollision)
				break;
		}
		if(onCollision)
			continue;
		for(Spider* spider : spiders){
			onCollision = CircleOnCollision(position, radius, spider->GetPosition(), spider->GetRadius());
			if(onCollision)
				break;
		}
		if(onCollision)
			continue;
		for(Cactus* cactus : cactuses){
			onCollision = CircleOnCollision(position, radius, cactus->GetPosition(), cactus->GetRadius());
			if(onCollision)
				break;
		}
		if(onCollision)
			continue;*/
	}
	return position;
}
/*
bool GameScreen::SpiderOnCollision(){
	Point spiderAhead(spider->GetPosition().x, spider->GetPosition().y);
	spiderAhead.y += -spider->GetSpeedV() * sin(spider->GetAngle() / 180.0f * PI) * 0.1f;
	spiderAhead.x += spider->GetSpeedV() * cos(spider->GetAngle() / 180.0f * PI) * 0.1f;
	return snake->OnCollision(spiderAhead, spider->GetRadius());
}*/

void GameScreen::OnMenuClick(){
	game_over->Stop();
	game->SetScreen(new MenuScreen(game));
}

void GameScreen::OnResumeClick(){
	state = GameState::RUNNING;
}

void GameScreen::OnRestartClick(){
	Restart();
}
