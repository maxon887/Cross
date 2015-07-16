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
	speedV = 130.f;
	target_apple = NULL;
	run_snd = NULL;
	hungry = true;
	temporary_run = false;
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
		x = -GetRadius();
		y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		break;
	case 1:		//bottom
		x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		y =	game->GetHeight() + GetRadius();
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		destanation.y = game->GetHeight() / 2 + rand() % (int)(game->GetHeight() / 2);
		break;
	case 2:		//right
		x = game->GetWidth() + GetRadius();
		y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		destanation.x = game->GetWidth() / 2 + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
		break;
	case 3:		//top
		x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
		y = -GetRadius();
		destanation.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
		destanation.y = GetRadius() + rand() % (int)(game->GetHeight() / 2);
		break;
	default:
		break;
	}
	SetPosition(Point(x, y));
	angle = Angle(GetPosition(), destanation);
	hungry = true;
	if(game->IsSoundEnabled()){
		run_snd = new Audio("Game/Spider/SpiderRun.wav", true, false);
	}
	run_snd->Play();
	state = SpiderState::RUNNING;
	anim = new Animation(*st_anim);
	anim->Start();
}

Spider::~Spider(){
	delete run_snd;
	free(anim);
	for(Collisioner* col : radars){
		delete col;
	}
}

void Spider::Update(float sec){
	Collisioner::Update(sec);
	switch (state)
	{
	case SpiderState::RUNNING:{
		anim->Update(sec);

		float sinus = sin((angle + 90) / 180.f * PI);
		float cosinus = cos((angle + 90) / 180.f * PI);
		for(int i = 0; i < 4; i++){
			float x = sinus * (i + 1) * 50 + GetPosition().x;
			float y = cosinus * (i + 1) * 50 + GetPosition().y;
			radars[i]->Update(sec);
			radars[i]->SetPosition(Point(x, y));
		}

		graphics->DrawCircle(destanation, 15, Color::Blue);
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
		}break;
	case SpiderState::THINKING:
		break;
	case SpiderState::ROTATING:
		break;
	case SpiderState::HIDING:
		break;
	case SpiderState::DEAD:
		break;
	default:
		break;
	}
}

void Spider::NearRadarCollision(){
	speedV = 0;
}

void Spider::FarRadarCollision(){
	bool haveNewDirection = false;
	float deltaAngle = 5;
	int j = 0;
	while(speedV != 0 && !haveNewDirection && j < 18){
		j++;
		for(int i = 0; i < 2; i++){
			float newAngle = angle + deltaAngle + 90;
			float x = 0;
			float y = 0;
			for(int j = 0; j < 4; j++){
				x = sin(newAngle / 180.f * PI) * (j + 1) * 50 + GetPosition().x;
				y = cos(newAngle / 180.f * PI) * (j + 1) * 50 + GetPosition().y;	
				haveNewDirection = screen->IsEmptyPosition(Point(x, y), GetRadius());
				if(!haveNewDirection)
					break;
			}
			if(haveNewDirection){
				destanation.x = x;
				destanation.y = y;
				break;
			}
			deltaAngle *= -1;
		}
		deltaAngle += 10;
		if(haveNewDirection)
			break;
	}
}

vector<Collisioner*>& Spider::GetRadars(){
	return radars;
}

void Spider::Draw(){
	switch (state){
	case SpiderState::RUNNING:
	case SpiderState::ROTATING:
		graphics->Rotate(anim->GetImage(), angle + 90.f);
		graphics->DrawImage(GetPosition(), anim->GetImage());
		break;
	case SpiderState::THINKING:{
			graphics->Rotate(body, angle + 90.f);
			graphics->Rotate(head, angle + head_angle + 90.f);
			graphics->DrawImage(GetPosition(), body);
			Point headPos;
			headPos.y = GetPosition().y + (float)-10 * sin(angle / 180.f * PI);
			headPos.x = GetPosition().x + (float)10 * cos(angle / 180.f * PI);
			graphics->DrawImage(headPos, head);
		}break;
	case SpiderState::DEAD:
		break;
	default:
		break;
	}
	for(Collisioner* col : radars){
		graphics->DrawCircle(col->GetPosition(), col->GetRadius(), Color::Red);
	}
	graphics->DrawLine(radars[0]->GetPosition(), radars[3]->GetPosition(), Color::Red);
}

float Spider::GetRadius(){
	return 24.f;
}
/*
bool Spider::OnScreen(){
	if(GetPosition().x > game->GetWidth() || GetPosition().x < 0)
		return false;
	if(GetPosition().y > game->GetHeight() || GetPosition().y < 0)
		return false;
	return true;
}*/

void Spider::ScanForApples(list<Apple*> &apples){
	Point p1;
	p1.y = GetPosition().y + (float)-2000 * sin((angle + 45.f) / 180.f * PI);
	p1.x = GetPosition().x + (float)2000 * cos((angle + 45.f) / 180.f * PI);
	Point p2;
	p2.y = GetPosition().y + (float)-2000 * sin((angle - 45.f) / 180.f * PI);
	p2.x = GetPosition().x + (float)2000 * cos((angle - 45.f) / 180.f * PI);
	//graphics->DrawLine(GetPosition(), p1, Color::Red);
	//graphics->DrawLine(GetPosition(), p2, Color::Red);
	target_apple = NULL;
	for(Apple* apple : apples){
		if(PointInTriangle(apple->GetPosition(), GetPosition(), p1, p2)){
			if(target_apple == NULL){
				target_apple = apple;
				destanation = target_apple->GetPosition();
			}else{
				float targedDistance = Distance(target_apple->GetPosition(), GetPosition());
				float newDistatnce = Distance(apple->GetPosition(), GetPosition());
				if(newDistatnce < targedDistance){
					target_apple = apple;
					destanation = target_apple->GetPosition();
				}
			}
		}
	}
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

void Spider::EatApple(list<Apple*> &apples){
	for(auto it = apples.begin(); it != apples.end(); it++){
		if((*it) == target_apple){
			delete *it;
			*it = NULL;
			it = apples.erase(it);
			target_apple = NULL;
			hungry = false;
			return;
		}
	}
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