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
	graphics->ReleaseImage(pause_img);
	graphics->ReleaseImage(ready_img);
	graphics->ReleaseImage(best_img);

	delete back_btn;
	delete restart_btn;
	delete menu_btn;
	delete pause_btn;
	delete left_btn;
	delete right_btn;

	delete music;
	delete game_over;
	delete apple_drop;
	delete btn_push;
	delete btn_pull;

	delete score_texter;
	delete score_texter_light;

	Snake::Release();
	Spider::Release();
	Apple::Release();
	Cactus::Release();
}
//						OVERRIDDEN METHODS
void GameScreen::Start(){
	srand((unsigned int)time(NULL));
	input->KeyPressed.Clear();
	input->KeyPressed += MakeDelegate(this, &GameScreen::KeyPressedHandler);
	input->ActionDown.Clear();
	input->ActionDown += MakeDelegate(this, &GameScreen::ActionDownHandler);
	input->ActionMove.Clear();
	input->ActionMove += MakeDelegate(this, &GameScreen::ActionMoveHandler);
	input->ActionUp.Clear();
	input->ActionUp += MakeDelegate(this, &GameScreen::ActionUpHandler);
	GameObject::Init(game);
	Snake::Init();
	Apple::Init();
	Spider::Init();
	Cactus::Init();
	snake = NULL;
	music = NULL;
	apple_drop = NULL;
	btn_push = NULL;
	btn_pull = NULL;
	game_over = NULL;
	commercial = NULL;
	going_menu = false;
	on_left = false;
	on_right = false;
	//image loading
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50.f, game->GetHeight() + 50.f);
	ready_img				= graphics->LoadImage("Game/ReadyLabel.png");
	score_img				= graphics->LoadImage("Game/ScoreLabel.png");
	score_bright_img		= graphics->LoadImage("Game/ScoreBrightLabel.png");
	pause_img				= graphics->LoadImage("Game/PauseLabel.png");
	gameover_img			= graphics->LoadImage("Game/GameOverLabel.png");
	best_img				= graphics->LoadImage("Game/Best.png");
	Image* backUp			= graphics->LoadImage("Game/BackUp.png");
	Image* backDown			= graphics->LoadImage("Game/BackDown.png");
	Image* menuUp			= graphics->LoadImage("Game/MenuUp.png");
	Image* menuDown			= graphics->LoadImage("Game/MenuDown.png");
	Image* restartUp		= graphics->LoadImage("Game/RestartUp.png");
	Image* restartDown		= graphics->LoadImage("Game/RestartDown.png");
	Image* pauseUp			= graphics->LoadImage("Game/PauseUp.png");
	Image* pauseDown		= graphics->LoadImage("Game/PauseDown.png");
	Image* arrowRightUp		= graphics->LoadImage("Game/ArrowUp.png");
	Image* arrowRightDown	= graphics->LoadImage("Game/ArrowDown.png");
	Image* arrowLeft = new Image(*arrowRightUp);
	graphics->Rotate(arrowLeft, 180.f);
	Image* arrowLeftPressed = new Image(*arrowRightDown);
	graphics->Rotate(arrowLeftPressed, 180.f);

	centerW = game->GetWidth() / 2;
	centerH = game->GetHeight() / 2;
	if(game->IsMusicEnabled()){
		music = new Audio("Game/GameMusic.mp3", true, true);
		game_over = new Audio("Game/GameOver.mp3", false, true);
	}
	if(game->IsSoundEnabled()){
		apple_drop = new Audio("Game/AppleDrop.wav", false, false);
		btn_push = new Audio("ButtonPush.wav", false, false);
		btn_pull = new Audio("ButtonPull.wav", false, false);
	}

	pause_btn = new Button(game, pauseUp, pauseDown);
	pause_btn->SetLocation(Point(pause_btn->GetWidth()/3*2, pause_btn->GetHeight()/3*2));
	pause_btn->RegisterCallback(bind(&GameScreen::OnPauseClick, this));
	pause_btn->SetSounds(btn_push, btn_pull);
	back_btn = new Button(game, backUp, backDown);
	back_btn->SetLocation(Point(450, centerH - 40));
	back_btn->RegisterCallback(bind(&GameScreen::OnResumeClick, this));
	back_btn->SetSounds(btn_push, btn_pull);
	menu_btn = new Button(game, menuUp, menuDown);
	menu_btn->SetLocation(Point(450, centerH + 180));
	menu_btn->RegisterCallback(bind(&GameScreen::OnMenuClick, this));
	menu_btn->SetSounds(btn_push, btn_pull);
	restart_btn = new Button(game, restartUp, restartDown);
	restart_btn->SetLocation(Point(450, centerH - 40));
	restart_btn->RegisterCallback(bind(&GameScreen::OnRestartClick, this)); 
	restart_btn->SetSounds(btn_push, btn_pull);
	right_btn = new Button(game, arrowRightUp, arrowRightDown);
	right_btn->SetLocation(Point(game->GetWidth() - arrowRightUp->GetWidth()/2, game->GetHeight() - arrowRightUp->GetHeight()/2));
	left_btn = new Button(game, arrowLeft, arrowLeftPressed);
	left_btn->SetLocation(Point(arrowLeft->GetWidth()/2, game->GetHeight() - arrowLeft->GetHeight()/2));

	control = game->GetControl();
	score_texter = new Texter(game, "Game/NumbersRed.png", 60.f, 76.f, 10, 1, 48);
	score_texter_light = new Texter(game, "Game/NumbersRedLight.png", 65.f, 76.f, 10, 1, 48);
	if(!game->IsPurchased()){
		commercial = game->GetCommercial();
	}
	Restart();
}

void GameScreen::Update(float sec){
    if(sec > 0.1){
        launcher->LogIt("Warning! Update time more than 0.1 sec");
        return;
    }
	graphics->Clear(0.25, 0.25, 0);
	graphics->DrawImage(game->GetWidth()/2, game->GetHeight()/2, background);
	if(going_menu){
		game->SetScreen(new MenuScreen(game));
		return;
	}
	ProccessCollisions();
	UpdateApples(sec);
	DrawApples();
	UpdateSpiders(sec);
	DrawSpiders();
	UpdateCactuses(sec);
	DrawCactuses();
	snake->Update(sec);
	snake->Draw();
	switch (state) {
	case GameState::ONREADY:
		graphics->DrawImage(centerW, game->GetHeight() / 3, ready_img);
		onready_time -= sec;
		if(onready_time < 0){
			state = GameState::RUNNING;
		}
		break;
	case GameState::RUNNING:{
		if(!pause_btn->IsPressed()){
			CalcInput(sec);
		}
		DrawScore();
		pause_btn->Update();
		}break;
	case GameState::PAUSED:{
		DrawScore();
		graphics->DrawImage(450, centerH - 250, pause_img);
		back_btn->Update();
		menu_btn->Update();
		}break;
	case GameState::GAMEOVER:
		graphics->DrawImage(450, centerH - 340, gameover_img);
		DrawLastScore();
		if(ad_timer > 0){
			ad_timer -= sec;
			if(ad_timer <= 0){
				if(commercial){
					if(ad_downloaded && !game->IsPurchased()){
						commercial->ShowAd();
					}
				}
			}
		}
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
		back_btn->SetActive(false);
		menu_btn->SetActive(false);
		for(Spider* spider : spiders){
			spider->Resume();
		}
		break;
	case GameState::PAUSED:
		music->Pause();
		back_btn->SetActive(true);
		menu_btn->SetActive(true);
		for(Spider* spider : spiders){
			spider->Pause();
		}
		break;
	case GameState::GAMEOVER:
		game_over->Play();
		restart_btn->SetActive(true);
		menu_btn->SetActive(true);
		if(score > game->BestScore()){
			game->SetBestScore(score);
			is_best_score = true;
		}
		break;
	default:
		throw string("Unexpected GameState");
	}
	state = newState;
}

void GameScreen::AddScore(int gain){
	score += gain;
	if(score > 10 && !ad_downloaded && commercial && !game->IsPurchased()){
		commercial->DownloadAd();
		ad_downloaded = true;
	}
}

void GameScreen::StopMusic(){
	music->Stop();
}

//						PRIVATE METHODS	
void GameScreen::Restart(){
	is_best_score = false;
	ad_downloaded = false;
	score = 0;
	onready_time = 4.3f;
	ad_timer = 0.7f;
	game_over->Stop();
	music->Play();
	score_texter->SetScaleFactor(game->GetScaleFactor());
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

	back_btn->SetActive(false);
	menu_btn->SetActive(false);
	restart_btn->SetActive(false);
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
	Point labelPos;
	Point texterPos;
	int digits = 0;
	int locScore = score;
	if(locScore != 0){
		while(locScore){
			locScore /= 10;
			digits++;
		}
	}else{
		digits = 1;
	}
	labelPos.x = game->GetWidth() - score_img->GetWidth()/2 - score_texter->GetWidth() * (digits + 1);
	labelPos.y = 50;
	texterPos.x = game->GetWidth() - (score_texter->GetWidth() * digits) - 10.f;
	texterPos.y = 20;
	graphics->DrawImage(labelPos, score_img);
	score_texter->DrawText(texterPos, to_string(score));
}

void GameScreen::DrawLastScore(){
	if(is_best_score){
		Point bestPos;
		Point labelPos;
		Point texterPos;
		int digits = 0;
		int locScore = score;
		if(locScore != 0){
			while(locScore){
				locScore /= 10;
				digits++;
			}
		}else{
			digits = 1;
		}
		bestPos.x = game->GetWidth() - best_img->GetWidth() / 2;
		bestPos.y = centerH - 220;
		texterPos.x = 620 - score_texter->GetWidth() * (digits + 1);
		texterPos.y = centerH - 250;
		labelPos.x = 400 - score_texter->GetWidth() * (digits + 1);
		labelPos.y = centerH - 210;

		if(is_best_score){
			graphics->DrawImage(bestPos, best_img);
		}
		graphics->DrawImage(labelPos, score_bright_img);
		score_texter_light->DrawText(texterPos, to_string(score));
	}else{
		graphics->DrawImage(300, centerH - 210, score_bright_img);
		score_texter_light->DrawText(520, centerH - 255, to_string(score));
	}
}

void GameScreen::CalcInput(float sec){
	if(control_pos != input_pos){
		float fingerAngle = Angle(control_pos, input_pos);
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
			if(counterclockwise < clockwise) {
				snake->Rotate(snake->Direction() + snake->GetSpeedW() * sec);
			}else{
				snake->Rotate(snake->Direction() - snake->GetSpeedW() * sec);
			}

			if(snake->Direction() >= 180.f)
				snake->Rotate(snake->Direction() - 360.f);
			if(snake->Direction() <= -180.f)
				snake->Rotate(snake->Direction() + 360.f);
		}
	}

	if(control == ARROWS){
		if(on_left){
			snake->Rotate(snake->Direction() + snake->GetSpeedW() * sec);
			left_btn->DrawDown();
		}else{
			left_btn->DrawUp();
		}

		if(on_right){
			snake->Rotate(snake->Direction() - snake->GetSpeedW() * sec);
			right_btn->DrawDown();
		}else{
			right_btn->DrawUp();
		}
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
	if(!snake->IsDead()){
		if(score > game->BestScore()){
			game->SetBestScore(score);
		}
	}
	//game->SetScreen(new MenuScreen(game));
	going_menu = true;
}

void GameScreen::OnResumeClick(){
	SetState(GameState::RUNNING);
}

void GameScreen::OnRestartClick(){
	Restart();
}

void GameScreen::OnPauseClick(){
	SetState(GameState::PAUSED);
}

void GameScreen::KeyPressedHandler(Key key){
	switch(state){
	case GameState::RUNNING:
		SetState(GameState::PAUSED);
		break;
	case GameState::PAUSED:
		SetState(GameState::RUNNING);
		break;
	}
}

void GameScreen::ActionDownHandler(Point pos){
	if(control == SLIDE){
		control_pos.x = pos.x;
		control_pos.y = pos.y;
	}
	if(control == ARROWS){
		if(left_btn->OnLocation(pos)){
			on_left = true;
		}
		if(right_btn->OnLocation(pos)){
			on_right = true;
		}
	}
}

void GameScreen::ActionMoveHandler(Point pos){
	if(state == GameState::ONREADY){
		state = GameState::RUNNING;
	}

	if(control == SLIDE){
		static const float NeedDistance = 300.f;
		float RealDistance = Distance(control_pos, pos);
		if(RealDistance > NeedDistance){
			float fingerAngle = Angle(control_pos, pos);
			float deltaDist = RealDistance - NeedDistance;
			control_pos.x += deltaDist * cos(fingerAngle / 180.f * PI);
			control_pos.y += -deltaDist * sin(fingerAngle / 180.f * PI);
		}
		input_pos = pos;
	}

	if(control == ARROWS){
		if(left_btn->OnLocation(pos)){
			on_left = true;
			on_right = false;
		}
		if(right_btn->OnLocation(pos)){
			on_right = true;
			on_left = false;
		}
	}
}

void GameScreen::ActionUpHandler(Point pos){
	if(control == SLIDE){
		input_pos = pos;
		control_pos = pos;
	}
	if(control == ARROWS){
		on_left = false;
		on_right = false;
	}
}
