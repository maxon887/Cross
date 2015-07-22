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
	
#include "Spider.h"
#include "Snake.h"
#include "Cactus.h"
#include "Misc.h"
#include <stdlib.h>
#include <cmath>

Animation* Spider::st_anim = NULL;
Image* Spider::head = NULL;
Image* Spider::body = NULL;

const float Spider::speedW = 90.f;

void Spider::Init(){
	Image* frames[8];
	frames[0] = graphics->LoadImage("Game/Spider/00.png");
	frames[1] = graphics->LoadImage("Game/Spider/01.png");
	frames[2] = graphics->LoadImage("Game/Spider/02.png");
	frames[3] = graphics->LoadImage("Game/Spider/03.png");
	frames[4] = graphics->LoadImage("Game/Spider/04.png");
	frames[5] = graphics->LoadImage("Game/Spider/05.png");
	frames[6] = graphics->LoadImage("Game/Spider/06.png");
	frames[7] = graphics->LoadImage("Game/Spider/07.png");
	st_anim = new Animation(graphics, 0.03f, frames, 8, true);
	head = graphics->LoadImage("Game/Spider/Head.png");
	body = graphics->LoadImage("Game/Spider/Body.png");
}

void Spider::Release(){
	delete st_anim;
	graphics->ReleaseImage(head);
	graphics->ReleaseImage(body);
}

Spider::Spider(){
	head_angle = 0;
	thinking_time = 1.3f;
	eat_time = .8f;
	speedV = 130.f;
	think_turn_left = false;
	eat_apple = NULL;
	run_snd = NULL;
	eat_snd = NULL;
	hungry = true;
	temporary_run = false;

	if(game->IsSoundEnabled()){
		run_snd = new Audio("Game/Spider/SpiderRun.wav", true, false);
		eat_snd = new Audio("Game/Spider/SpiderEat.wav", false, false);
	}
	run_snd->Play();
	state = SpiderState::RUNNING;
	anim = new Animation(*st_anim);
	anim->Start();
	//Radars initialization
	float sinus = sin((angle + 90) / 180.f * PI);
	float cosinus = cos((angle + 90) / 180.f * PI);
	for(int i = 0; i < 4; i++){
		float x = sinus * (i + 1) * 50 + GetPosition().x;
		float y = cosinus * (i + 1) * 50 + GetPosition().y;
		if(i < 2){
			radars.push_back(new NearRadar(Point(x, y), this));
		}else{
			radars.push_back(new FarRadar(Point(x, y), this));
		}
	}

	short side = rand()%4;
	float x, y;
	switch (side){
	case 0:		//left
		//x = -GetRadius();
		x = -GetTurnRadius();
		y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		break;
	case 1:		//bottom
		x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		//y =	game->GetHeight() + GetRadius();
		y = game->GetHeight() + GetTurnRadius();
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		destanation.y = game->GetHeight() / 2 + rand() % (int)(game->GetHeight() / 2);
		break;
	case 2:		//right
		//x = game->GetWidth() + GetRadius();
		x = game->GetWidth() + GetTurnRadius();
		y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		destanation.x = game->GetWidth() / 2 + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		break;
	case 3:		//top
		x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		//y = -GetRadius();
		y = -GetTurnRadius();
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() / 2);
		break;
	default:
		break;
	}
	SetPosition(Point(x, y));
	angle = Angle(GetPosition(), destanation);
}
/*
Spider::Spider(Point position, Point destanation):Eatable(position){
	this->destanation = destanation;
	head_angle = 0;
	thinking_time = 1.3f;
	speedV = 130.f;
	eat_time = .8f;
	eat_apple = NULL;
	run_snd = NULL;
	hungry = true;
	temporary_run = false;

	if(game->IsSoundEnabled()){
		run_snd = new Audio("Game/Spider/SpiderRun.wav", true, false);
	}
	run_snd->Play();
	state = SpiderState::RUNNING;
	anim = new Animation(*st_anim);
	anim->Start();
	//Radars initialization
	float sinus = sin((angle + 90) / 180.f * PI);
	float cosinus = cos((angle + 90) / 180.f * PI);
	for(int i = 0; i < 4; i++){
		float x = sinus * (i + 1) * 50 + GetPosition().x;
		float y = cosinus * (i + 1) * 50 + GetPosition().y;
		if(i < 2){
			radars.push_back(new NearRadar(Point(x, y), this));
		}else{
			radars.push_back(new FarRadar(Point(x, y), this));
		}
	}
}*/

Spider::~Spider(){
	delete run_snd;
	delete anim;
	for(Collisioner* col : radars){
		delete col;
	}
}

void Spider::Update(float sec){
	Collisioner::Update(sec);

	float sinus = sin((angle + 90) / 180.f * PI);
	float cosinus = cos((angle + 90) / 180.f * PI);
	for(int i = 0; i < 4; i++){
		float x = sinus * (i + 1) * 50 + GetPosition().x;
		float y = cosinus * (i + 1) * 50 + GetPosition().y;
		radars[i]->Update(sec);
		radars[i]->SetPosition(Point(x, y));
	}

	switch (state){
	case SpiderState::RUNNING:{
		anim->Update(sec);

		graphics->DrawCircle(destanation, GetRadius(), Color::Blue);
		float neededAngle = Angle(GetPosition(), destanation);
		
		//determine in witch direction we need to move
		float clockwise;
		if( neededAngle > angle )
			clockwise = 360 + angle - neededAngle;
		else 
			clockwise = angle - neededAngle;
		float counterclockwise = 360 - clockwise;
		//rotate 
		if(clockwise < speedW * sec || counterclockwise < speedW * sec) {
			angle = neededAngle;
			speedV = 130.f;
		} else {
			if(counterclockwise < clockwise) {
				angle += speedW * sec;
			}else{
				angle -= speedW * sec;
			}
			if(angle >= 180.f)
				angle -= 360.f;
			if(angle <= -180.f)
				angle += 360.f;
		}
		Point pos = GetPosition();
		pos.y += (float)-speedV * sin(angle / 180.f * PI) * sec;
		pos.x += (float)speedV * cos(angle / 180.f * PI) * sec;
		SetPosition(pos);
		if(CircleOnCollision(pos, 2.f, destanation, 2.f)){
			state = THINKING;
			run_snd->Pause();
		}
		}break;
	case SpiderState::THINKING:{
		if(!OnScreen()){
			state = DEAD;
		}
		thinking_time -= sec;
		if(think_turn_left) {
			head_angle += sec * 150.f;
			if(head_angle > 45)
				think_turn_left = false;
		} else {
			head_angle -= sec * 150.f;
			if(head_angle < -45)
				think_turn_left = true;
		}
		if(thinking_time < 0){
			thinking_time = 1.3f;
			head_angle = 0;
			state = SpiderState::RUNNING;
			run_snd->Play();
			if(hungry){
				ScanForApples();
			}else{
				SetNearestBorder();
			}
		}
		}break;
	case SpiderState::EATING:
		if(eat_time < 0){
			eat_apple->Eat();
			hungry = false;
			state = THINKING;
		}else{
			eat_time -= sec;
		}
		break;
	case SpiderState::DEAD:
		break;
	default:
		break;
	}
}

void Spider::Pause(){
	run_snd->Pause();
}

void Spider::Resume(){
	if(state == RUNNING){
		run_snd->Resume();
	}
}

vector<Collisioner*>& Spider::GetRadars(){
	return radars;
}

void Spider::NearRadarCollision(){
	speedV = 0;
	FarRadarCollision();
}

void Spider::FarRadarCollision(){
	bool haveNewDirection = false;
	float deltaAngle = 5;
	while(!haveNewDirection && deltaAngle < 180){
		for(int i = 0; i < 2; i++){
			float newAngle = angle + deltaAngle + 90;
			Point dest;
			for(int j = 0; j < 4; j++){
				dest.x = sin(newAngle / 180.f * PI) * (j + 1) * 50 + GetPosition().x;
				dest.y = cos(newAngle / 180.f * PI) * (j + 1) * 50 + GetPosition().y;	
				haveNewDirection = screen->IsEmptyPosition(dest, GetRadius());
				if(!haveNewDirection)
					break;
			}
			if(haveNewDirection){
				destanation = dest;
				break;
			}
			deltaAngle *= -1;
		}
		if(haveNewDirection)
			break;
		else
			deltaAngle += 10;
	}
}

float Spider::GetTurnRadius(){
	return speedV / (speedW / 360.f * PI);
}

void Spider::Draw(){
	switch (state){
	case RUNNING:
	case EATING:
		graphics->Rotate(anim->GetImage(), angle + 90.f);
		graphics->DrawImage(GetPosition(), anim->GetImage());
		break;
	case THINKING:{
			graphics->Rotate(body, angle + 90.f);
			graphics->Rotate(head, angle + head_angle + 90.f);
			graphics->DrawImage(GetPosition(), body);
			Point headPos;
			headPos.y = GetPosition().y + (float)-10 * sin(angle / 180.f * PI);
			headPos.x = GetPosition().x + (float)10 * cos(angle / 180.f * PI);
			graphics->DrawImage(headPos, head);
		}break;
	case DEAD:
		break;
	default:
		break;
	}
	for(Collisioner* col : radars){
		graphics->DrawCircle(col->GetPosition(), col->GetRadius(), Color::Red);
	}
	graphics->DrawLine(radars[0]->GetPosition(), radars[3]->GetPosition(), Color::Red);
	graphics->DrawCircle(GetPosition(), GetTurnRadius(), Color::Yellow);
	graphics->DrawCircle(GetPosition(), GetRadius(), Color::Red);
}

float Spider::GetRadius(){
	return 24.f;
}

void Spider::CollisionOccurred(Collisioner* obj){
	Apple* apple = dynamic_cast<Apple*>(obj);
	if(apple){
		eat_apple = apple;
		state = EATING;
		eat_snd->Play();
		run_snd->Pause();
	}
}

void Spider::ObjectLeft(Collisioner* obj){
	if(obj == eat_apple){
		eat_apple = NULL;
		state = THINKING;
	}
}

bool Spider::OnScreen(){
	if(GetPosition().x > game->GetWidth() || GetPosition().x < 0)
		return false;
	if(GetPosition().y > game->GetHeight() || GetPosition().y < 0)
		return false;
	return true;
}

void Spider::ScanForApples(){
	list<Apple*> apples = screen->GetApples();
	Point dest(-10000, 10000);
	for(Apple* apple : apples){
		if(!CircleOnCollision(GetPosition(), GetTurnRadius(), apple->GetPosition(), apple->GetRadius())){
			if(Distance(GetPosition(), apple->GetPosition()) < Distance(GetPosition(), dest)){
				dest = apple->GetPosition();
			}
		}
	}
	if(dest == Point(-10000, 10000)){
		do{
			dest = screen->GetEmptyPosition(GetRadius());
		}while(Distance(dest, GetPosition()) < GetTurnRadius());
	}
	destanation = dest;
}

void Spider::SetNearestBorder(){
	destanation.x = GetPosition().x;
	destanation.y = - GetRadius();
	Point newPoint;
	float distance;
	float newDistance;

	newPoint.x = game->GetWidth() + GetRadius();
	newPoint.y = GetPosition().y;
	distance = Distance(destanation, GetPosition());
	newDistance = Distance(newPoint, GetPosition());
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		destanation = newPoint;

	newPoint.x = GetPosition().x;
	newPoint.y = game->GetHeight() + GetRadius();
	distance = Distance(destanation, GetPosition());
	newDistance = Distance(newPoint, GetPosition());
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		destanation = newPoint;

	newPoint.x = - GetRadius();
	newPoint.y = GetPosition().y;
	distance = Distance(destanation, GetPosition());
	newDistance = Distance(newPoint, GetPosition());
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		destanation = newPoint;
}

int Spider::Eat(){
	SetPosition(Point(-100, -100));;
	run_snd->Stop();
	state = SpiderState::DEAD;
	return 3;
}

bool Spider::Eaten(){
	if(state == SpiderState::DEAD)
		return true;
	else
		return false;
}