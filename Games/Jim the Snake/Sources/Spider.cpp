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
#include "Misc.h"
#include "JimTheSnake.h"
#include <stdlib.h>

Game* Spider::game = NULL;
Graphics* Spider::graphics = NULL;
Animation* Spider::anim = NULL;
Image* Spider::head = NULL;
Image* Spider::body = NULL;
Audio* Spider::run_snd = NULL;

const float Spider::radius = 24.f;
const float Spider::speedV = 130.f;
const float Spider::speedW = 90.f;

void Spider::Init(Game* game){
	Spider::game = game;
	Spider::graphics = game->graphics;
	Image* frames[8];
	frames[0] = graphics->LoadImage("Game/Spider/00.png");
	frames[1] = graphics->LoadImage("Game/Spider/01.png");
	frames[2] = graphics->LoadImage("Game/Spider/02.png");
	frames[3] = graphics->LoadImage("Game/Spider/03.png");
	frames[4] = graphics->LoadImage("Game/Spider/04.png");
	frames[5] = graphics->LoadImage("Game/Spider/05.png");
	frames[6] = graphics->LoadImage("Game/Spider/06.png");
	frames[7] = graphics->LoadImage("Game/Spider/07.png");
	anim = new Animation(graphics, 0.03f, frames, 8, true);
	head = graphics->LoadImage("Game/Spider/Head.png");
	body = graphics->LoadImage("Game/Spider/Body.png");
	if(game->saver->LoadBool(PROPERTY_SOUND)){
		run_snd = new Audio("Game/Spider/SpiderRun.wav", true, false);
	}else{
		run_snd = NULL;
	}
	anim->Start();
}

void Spider::Release(){
	delete anim;
	delete run_snd;
	graphics->ReleaseImage(head);
	graphics->ReleaseImage(body);
}

Spider::Spider(){
	if(game == NULL)
		throw string("Class Spider needs to be initialized staticly");
	state = SpiderState::DEAD;
	head_angle = 0;
	rotate_angle = 0;
	thinking_time = 1.3f;
	target_apple = NULL;
	eaten = false;
	pos.x = -100;
	pos.y = -100;
}

void Spider::Update(float sec, list<Apple*> &apples){
	switch (state)
	{
	case SpiderState::RUNNING:{
		anim->Update(sec);
		float neededAngle = Angle(pos, end_point);
		
		//determine in witch direction we need to move
		float clockwise;
		if( neededAngle > angle )
			clockwise = 360 + angle - neededAngle;
		else 
			clockwise = angle - neededAngle;
		float counterclockwise = 360 - clockwise;
		//rotate snake head
		if(clockwise < speedW * sec || counterclockwise < speedW * sec) {
			angle = neededAngle;
		} else {
			if(counterclockwise < clockwise) 
				angle += speedW * sec;
			else
				angle -= speedW * sec;
			if(angle >= 180.f)
				angle -= 360.f;
			if(angle <= -180.f)
				angle += 360.f;
		}

		//graphics->DrawCircle(end_point, 30, ColorX::Red);
		pos.y += (float)-speedV * sin(angle / 180.f * PI) * sec;
		pos.x += (float)speedV * cos(angle / 180.f * PI) * sec;
		if(CircleOnCollision(pos, 2, end_point, 2)){
			if(!eaten){
				if(target_apple != NULL){
					EatApple(apples);
				}
				state = SpiderState::THINKING;
				thinking_time = 1.3f;
			}else{
				state = SpiderState::DEAD;
			}
			run_snd->Stop();
		}
		break;}
	case SpiderState::THINKING:{
		static bool turn_left = false;
		thinking_time -= sec;
		if(turn_left) {
			head_angle += sec * 150.f;
			if(head_angle > 45)
				turn_left = false;
		} else {
			head_angle -= sec * 150.f;
			if(head_angle < -45)
				turn_left = true;
		}
		if(thinking_time < 0){
			head_angle = 0;
			state = SpiderState::RUNNING;
			run_snd->Play();
			if(!eaten){
				ScanForApples(apples);
				if(target_apple == NULL){
					do{
						end_point.x = GetRadius() + rand() % (int)(game->GetWidth() - GetRadius()*2);
						end_point.y = GetRadius() + rand() % (int)(game->GetHeight() - GetRadius()*2);
					}while(Distance(pos, end_point) <= speedV / (speedW / 360 * PI));
				}
			}else{
				SetNearestBorder();
			}
		}

		break;}
	case SpiderState::ROTATE:
		anim->Update(sec);
		if(angle < rotate_angle){
			angle += speedW * sec;
			if(angle > rotate_angle)
				angle = rotate_angle;
		}
		if(angle > rotate_angle){
			angle -= speedW * sec;
			if(angle < rotate_angle)
				angle = rotate_angle;
		}
		if(angle == rotate_angle){
			ScanForApples(apples);
			if(target_apple == NULL){
				do{
					end_point.x = GetRadius() + rand() % (int)(game->GetWidth() - GetRadius()*2);
					end_point.y = GetRadius() + rand() % (int)(game->GetHeight() - GetRadius()*2);
				}while(Distance(pos, end_point) <= speedV / (speedW / 360 * PI));
			}
			state = SpiderState::RUNNING;
		}
		break;
	case SpiderState::DEAD:
		break;
	default:
		throw string("Unexpected spider state");
		break;
	}
}

void Spider::Start(){
	if(state == SpiderState::DEAD){
		short side = rand()%4;
		float x, y;
		switch (side){
		case 0:		//left
			x = -GetRadius();
			y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
			end_point.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
			end_point.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
			break;
		case 1:		//bottom
			x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
			y =	game->GetHeight() + GetRadius();
			end_point.x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
			end_point.y = game->GetHeight() / 2 + rand() % (int)(game->GetHeight() / 2);
			break;
		case 2:		//right
			x = game->GetWidth() + GetRadius();
			y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
			end_point.x = game->GetWidth() / 2 + rand() % (int)(game->GetWidth() / 2);
			end_point.y = GetRadius() + rand() % (int)(game->GetHeight() - 2 * GetRadius());
			break;
		case 3:		//top
			x = GetRadius() + rand() % (int)(game->GetWidth() - 2 * GetRadius());
			y = -GetRadius();
			end_point.x = GetRadius() + rand() % (int)(game->GetWidth() / 2);
			end_point.y = GetRadius() + rand() % (int)(game->GetHeight() / 2);
			break;
		default:
			break;
		}
		pos.x = x;
		pos.y = y;
		angle = Angle(pos, end_point);
		run_snd->Play();
		eaten = false;
		state = SpiderState::RUNNING;
	}
}

void Spider::Draw(){
	switch (state)
	{
	case SpiderState::RUNNING:
	case SpiderState::ROTATE:
		graphics->Rotate(anim->GetImage(), angle + 90.f);
		graphics->DrawImage(pos, anim->GetImage());
		break;
	case SpiderState::THINKING:{
			graphics->Rotate(body, angle + 90.f);
			graphics->Rotate(head, angle + head_angle + 90.f);
			graphics->DrawImage(pos, body);
			PointX headPos;
			headPos.y = pos.y + (float)-10 * sin(angle / 180.f * PI);
			headPos.x = pos.x + (float)10 * cos(angle / 180.f * PI);
			graphics->DrawImage(headPos, head);
		}break;
	case SpiderState::DEAD:
		break;
	default:
		break;
	}
}

float Spider::GetRadius(){
	return radius;
}

PointX Spider::GetPosition(){
	return pos;
}

bool Spider::OnScreen(){
	if(pos.x > game->GetWidth() || pos.x < 0)
		return false;
	if(pos.y > game->GetHeight() || pos.y < 0)
		return false;
	return true;
}

void Spider::ScanForApples(list<Apple*> &apples){
	PointX p1;
	p1.y = pos.y + (float)-2000 * sin((angle + 45.f) / 180.f * PI);
	p1.x = pos.x + (float)2000 * cos((angle + 45.f) / 180.f * PI);
	PointX p2;
	p2.y = pos.y + (float)-2000 * sin((angle - 45.f) / 180.f * PI);
	p2.x = pos.x + (float)2000 * cos((angle - 45.f) / 180.f * PI);
	//graphics->DrawLine(pos, p1, ColorX::Red);
	//graphics->DrawLine(pos, p2, ColorX::Red);
	target_apple = NULL;
	for(Apple* apple : apples){
		if(PointInTriangle(apple->GetPosition(), pos, p1, p2)){
			if(target_apple == NULL){
				target_apple = apple;
				end_point = target_apple->GetPosition();
			}else{
				float targedDistance = Distance(target_apple->GetPosition(), pos);
				float newDistatnce = Distance(apple->GetPosition(), pos);
				if(newDistatnce < targedDistance){
					target_apple = apple;
					end_point = target_apple->GetPosition();
				}
			}
		}
	}
}

void Spider::SetNearestBorder(){
	end_point.x = pos.x;
	end_point.y = - GetRadius();
	PointX newPoint;
	float distance;
	float newDistance;

	newPoint.x = game->GetWidth() + GetRadius();
	newPoint.y = pos.y;
	distance = Distance(end_point, pos);
	newDistance = Distance(newPoint, pos);
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		end_point = newPoint;

	newPoint.x = pos.x;
	newPoint.y = game->GetHeight() + GetRadius();
	distance = Distance(end_point, pos);
	newDistance = Distance(newPoint, pos);
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		end_point = newPoint;

	newPoint.x = - GetRadius();
	newPoint.y = pos.y;
	distance = Distance(end_point, pos);
	newDistance = Distance(newPoint, pos);
	if(newDistance < distance && newDistance > speedV / (speedW / 360 * PI))
		end_point = newPoint;
}

void Spider::EatApple(list<Apple*> &apples){
	for(auto it = apples.begin(); it != apples.end(); it++){
		if((*it) == target_apple){
			delete *it;
			*it = NULL;
			it = apples.erase(it);
			target_apple = NULL;
			eaten = true;
			return;
		}
	}
}

int Spider::Eat(){
	pos.x = -100;
	pos.y = -100;
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

void Spider::Rotate(float deltaAngle){
	if(state != SpiderState::ROTATE){
		state = SpiderState::ROTATE;
		rotate_angle = angle + deltaAngle;
	}
}

float Spider::GetAngle(){
	return angle;
}

float Spider::GetSpeedV(){
	return speedV;
}