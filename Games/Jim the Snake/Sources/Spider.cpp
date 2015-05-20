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

Game* Spider::game = NULL;
Graphics* Spider::graphics = NULL;
Animation* Spider::anim = NULL;
Image* Spider::head = NULL;
Image* Spider::body = NULL;
Audio* Spider::run_snd = NULL;

const float Spider::radius = 24.f;
const float Spider::speedV = 130.f;
const float Spider::speedW = 120.f;

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
	run_snd = new Audio("Game/Spider/SpiderRun.wav", true, false);
	anim->Start();
}

Spider::Spider(){
	if(game == NULL)
		throw string("Class Spider needs to be initialized staticly");
	pos.x = -100;
	pos.y = -100;
	state = SpiderState::HIDING;
	head_angle = 0;
	thinking_time = 1.3f;
	target_apple = NULL;
	eaten = false;
}

void Spider::Update(float sec, list<Apple*> &apples){
	switch (state)
	{
	case SpiderState::RUNNING:{
		float neededAngle;
		float tangens = (pos.x - end_point.x) / (pos.y - end_point.y);
		neededAngle = atan(tangens);
		neededAngle = (float)(neededAngle * 180.f / PI) + 90.f;
		if(pos.x > end_point.x && pos.y < end_point.y){
			neededAngle += 180.f;
		}

		if(neededAngle != angle){
			if(neededAngle > angle){
				if(angle + 50 * sec > neededAngle){
					angle = neededAngle;
				}else{
					angle += 50 * sec;
				}
			}else{
				if(angle - 50 * sec < neededAngle){
					angle = neededAngle;
				}else{
					angle -= 50 * sec;
				}
			}
		}

		graphics->DrawCircle(end_point, 5, ColorX::Red);
		pos.y += -speedV * sin(angle / 180.0 * PI) * sec;
		pos.x += speedV * cos(angle / 180.0 * PI) * sec;
		anim->Update(sec);
		if(CircleOnCollision(pos, 2, end_point, 2)){
			if(!eaten){
				if(target_apple != NULL){
					EatApple(apples);
				}
				state = SpiderState::THINKING;
				thinking_time = 1.3f;
			}else{
				state = SpiderState::HIDING;
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
					end_point.x = GetRadius() + rand() % (int)(game->GetWidth() - GetRadius()*2);
					end_point.y = GetRadius() + rand() % (int)(game->GetHeight() - GetRadius()*2);
				}
			}else{
				SetNearestBorder();
			}
		}

		break;}
	case SpiderState::HIDING:
		break;
	default:
		throw string("Unexpected spider state");
		break;
	}
}

void Spider::Start(){
	if(state == SpiderState::HIDING){
		//short side = rand() % 4;
		short side = 1;
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
			end_point.y = game->GetHeight() / 2 + rand() % (int)(game->GetHeight() / 2);
			break;
		default:
			break;
		}/*
		float tangens = (x - end_point.x) / (y - end_point.y);
		angle = atan(tangens);
		angle = (float)(angle * 180.f / PI + 90.f);
		if(angle < -90){
			angle += 180.f;
		}
		if(angle > 90){
			angle -= 180.f;
		}*/
		pos.x = x;
		pos.y = y;
		angle = Angle(pos, end_point);
		run_snd->Play();
		state = SpiderState::RUNNING;
	}
}

void Spider::Draw(){
	switch (state)
	{
	case SpiderState::RUNNING:
		graphics->Rotate(anim->GetImage(), angle + 90.f);
		graphics->DrawImage(pos, anim->GetImage());
		break;
	case SpiderState::THINKING:{

		graphics->Rotate(body, angle + 90.f);
		graphics->Rotate(head, angle + head_angle + 90.f);
		graphics->DrawImage(pos, body);
		PointX headPos;
		headPos.y = pos.y + -10 * sin(angle / 180.0 * PI);
		headPos.x = pos.x + 10 * cos(angle / 180.0 * PI);
		graphics->DrawImage(headPos, head);
		break;}
	case SpiderState::ROTATE:
		break;
	case SpiderState::HIDING:
		break;
	default:
		break;
	}
}

float Spider::GetRadius(){
	return radius;
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
	p1.y = pos.y + -2000 * sin((angle + 45.f) / 180.0 * PI);
	p1.x = pos.x + 2000 * cos((angle + 45.f) / 180.0 * PI);
	PointX p2;
	p2.y = pos.y + -2000 * sin((angle - 45.f) / 180.0 * PI);
	p2.x = pos.x + 2000 * cos((angle - 45.f) / 180.0 * PI);
	graphics->DrawLine(pos, p1, ColorX::Red);
	graphics->DrawLine(pos, p2, ColorX::Red);
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
	if(newDistance < distance)
		end_point = newPoint;

	newPoint.x = pos.x;
	newPoint.y = game->GetHeight() + GetRadius();
	distance = Distance(end_point, pos);
	newDistance = Distance(newPoint, pos);
	if(newDistance < distance)
		end_point = newPoint;

	newPoint.x = - GetRadius();
	newPoint.y = pos.y;
	distance = Distance(end_point, pos);
	newDistance = Distance(newPoint, pos);
	if(newDistance < distance)
		end_point = newPoint;
}

void Spider::EatApple(list<Apple*> &apples){
	for(auto it = apples.begin(); it != apples.end(); it++){
		if((*it) == target_apple){
			delete *it;
			*it = NULL;
			apples.erase(it);
			target_apple = NULL;
			eaten = true;
			return;
		}
	}
}