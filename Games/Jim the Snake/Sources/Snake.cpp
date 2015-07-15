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
	
#include "Snake.h"
#include "Misc.h"
#include <stdlib.h>
#include <cmath>
#include "JimTheSnake.h"

void Snake::Radar::CollisionOccurred(Collisioner* obj){
	if(!face_bottom_anim->IsRunning() && !snake->dead){
		face_bottom_anim->Start();
		face_top_anim->Start();
		eat_snd->Play();
	}
}

const float		Snake::speedV			= 240.f;
const float		Snake::speedW			= speedV / 1.33f;
const float		Snake::nod_length		= 50.f;
const float		Snake::star_speedW		= 2.f;
Image*			Snake::star_img			= NULL;
Image*			Snake::face_dead		= NULL;
Animation*		Snake::face_bottom_anim	= NULL;
Animation*		Snake::face_top_anim	= NULL;
Audio*			Snake::eat_snd			= NULL;
Audio*			Snake::punch			= NULL;

//						STATIC METHODS
void Snake::Init(){
	Body::img = graphics->LoadImage("Game/Body.png");
	star_img = graphics->LoadImage("Game/Star.png");
	face_dead = graphics->LoadImage("Game/Face/FaceDead.png");

	Image* framesBottom[16];
	framesBottom[0] = graphics->LoadImage("Game/Face/00Bottom.png");
	framesBottom[1] = graphics->LoadImage("Game/Face/01Bottom.png");
	framesBottom[2] = graphics->LoadImage("Game/Face/02Bottom.png");
	framesBottom[3] = graphics->LoadImage("Game/Face/03Bottom.png");
	framesBottom[4] = graphics->LoadImage("Game/Face/04Bottom.png");
	framesBottom[5] = graphics->LoadImage("Game/Face/05Bottom.png");
	framesBottom[6] = graphics->LoadImage("Game/Face/06Bottom.png");
	framesBottom[7] = framesBottom[5];
	framesBottom[8] = framesBottom[4];
	framesBottom[9] = framesBottom[3];
	framesBottom[10] = framesBottom[2];
	framesBottom[11] = framesBottom[1];
	framesBottom[12] = framesBottom[0];
	framesBottom[13] = framesBottom[0];
	framesBottom[14] = framesBottom[0];
	framesBottom[15] = framesBottom[0];

	Image* framesTop[16];
	framesTop[0] = graphics->LoadImage("Game/Face/00Top.png");
	framesTop[1] = graphics->LoadImage("Game/Face/01Top.png");
	framesTop[2] = graphics->LoadImage("Game/Face/02Top.png");
	framesTop[3] = graphics->LoadImage("Game/Face/03Top.png");
	framesTop[4] = graphics->LoadImage("Game/Face/04Top.png");
	framesTop[5] = graphics->LoadImage("Game/Face/05Top.png");
	framesTop[6] = graphics->LoadImage("Game/Face/06Top.png");
	framesTop[7] = framesTop[5];
	framesTop[8] = framesTop[4];
	framesTop[9] = framesTop[3];
	framesTop[10] = framesTop[2];
	framesTop[11] = framesTop[1];
	framesTop[12] = framesTop[0];
	framesTop[13] = framesTop[0];
	framesTop[14] = framesTop[0];
	framesTop[15] = framesTop[0];

	face_bottom_anim = new Animation(graphics, 10.f / speedV, framesBottom, 16);
	face_top_anim = new Animation(graphics, 10.f / speedV, framesTop, 16);

	if(game->IsSoundEnabled()){
		eat_snd = new Audio("Game/Eat.wav", false, false);
		punch = new Audio("Game/Punch.wav", false, false);
	}
}

void Snake::Release(){
	graphics->ReleaseImage(Body::img);
	graphics->ReleaseImage(star_img);
	graphics->ReleaseImage(face_dead);
	delete face_bottom_anim;
	delete face_top_anim;
	delete eat_snd;
	delete punch;
}
//						CONSTRUCTOR
Snake::Snake():Collisioner(Point(200, 400)){
	angle = 0;
	star_angle = 0;
	eatable_time_left = -1;
	body_length = 150.f;
	body_length = 10000.f;
	dead_time = 0;
	dead = false;
	body_path.clear();
	body_nodes.clear();
	body_path.push_back(Point(0, 400));
	body_nodes.push_back(new Body(Point(-100, -100), 1.f));
	radar = new Radar(GetPosition(), this);
}
//						DESTRUCTOR
Snake::~Snake(){
	for(Body* b : body_nodes){
		delete b;
	}
	delete radar;
}
//						OVERRIDDEN METHODS
float Snake::GetRadius(){
	return 26.f;
}

void Snake::CollisionOccurred(Collisioner* obj){
	Body* b = dynamic_cast<Body*>(obj);
	if(b){
		Die();
	}
}
//						PUBLIC METHODS	
float Snake::Direction(){
	return angle;
}

void Snake::Rotate(float angle){
	if(!dead){
		this->angle = angle;
	}
}

void Snake::Update(float sec){
	Collisioner::Update(sec);
	switch (screen->GetState())
	{
	case GameState::ONREADY:
		UpdateBody(sec);
		break;
	case GameState::RUNNING:
		if(!dead){
			face_bottom_anim->Update(sec);
			face_top_anim->Update(sec);
			Point p = GetPosition();
			p.x += speedV * cos(angle / 180.f * PI) * sec;
			p.y += -speedV * sin(angle / 180.f * PI) * sec;
			SetPosition(p);
			radar->SetPosition(p);
			radar->Update(sec);
			UpdateBody(sec);

			Eatable* eatable = dynamic_cast<Eatable*>(radar->OnCollision());
			if(eatable){
				if(eatable_time_left > 0){
					eatable_time_left -= sec;
					if(eatable_time_left < 0){
						auto bigNode = body_nodes.begin();
						++bigNode;
						(*bigNode)->SetBig();
						screen->AddScore(eatable->Eat());
					}
				}else{
					if(CircleOnCollision(GetPosition(), GetRadius(), eatable->GetPosition(), 1.f)) {
						eatable_time_left = 0.025f;
					}
				}
			}
		}else{
			dead_time += sec;
		}
		break;
	case GameState::PAUSED:
		break;
	case GameState::GAMEOVER:
		star_angle += star_speedW * sec;
		break;
	default:
		break;
	}
}

void Snake::Draw(){
	if(!dead){
		Image* face = face_bottom_anim->GetImage();
		graphics->Rotate(face, angle + 90.f);
		graphics->DrawImage(GetPosition(), face);
		if(radar->OnCollision()){
			radar->OnCollision()->Draw();
		}
		DrawBody();
		face = face_top_anim->GetImage();
		graphics->Rotate(face, angle + 90.f);
		graphics->DrawImage(GetPosition(), face);
		graphics->DrawCircle(radar->GetPosition(), radar->GetRadius(), Color::Green);
	}else{
		DrawBody();
		graphics->Rotate(face_dead, angle + 90.f);
		graphics->DrawImage(GetPosition(), face_dead);
		if(dead_time > 1.f){
			Point stPos;
			for(int i = 0; i < 6; i++){
				stPos.x = (GetRadius() * cos(star_angle + i * 72.f / 180.f * PI));
				stPos.y = (GetRadius() * sin(star_angle + i * 72.f / 180.f * PI));
				stPos.x += GetPosition().x;
				stPos.y += GetPosition().y;
				graphics->DrawImage(stPos, star_img);
			}
			if(screen->GetState() != GameState::GAMEOVER){
				screen->SetState(GameState::GAMEOVER);
			}
		}
	}
	graphics->DrawCircle(GetPosition(), GetRadius(), Color::Red);
}

float Snake::GetSpeedW(){
	return speedW;
}

vector<Body*>& Snake::GetBodyNodes(){
	return body_nodes;
}

Collisioner* Snake::GetRadar(){
	return radar;
}
//						PRIVATE METHODS	

bool Snake::OnBorder(){
	Point p = GetPosition();
	if( p.x - GetRadius() < 0 ) {
		p.x = GetRadius();
		return true;
	}
	if( p.x + GetRadius() > game->GetWidth()) {
		p.x = game->GetWidth() - GetRadius();
		return true;
	}
	if( p.y - GetRadius() < 0 ) {
		p.y = GetRadius();
		return true;
	}
	if( p.y + GetRadius() > game->GetHeight()) {
		p.y = game->GetHeight() - GetRadius();
		return true;
	}
	return false;
}

void Snake::UpdateBody(float sec){
	Point first = *body_path.begin();
	if(first != GetPosition()){
		body_path.insert(body_path.begin(), GetPosition());
	}
	//drawing body nodes
	float pathLen = 0;
	unsigned int nodIndex = 1;
	auto nextIt = body_path.begin();
	auto endIt = body_path.end();
	Point curr;
	Point next = *nextIt;
	nextIt++;

	while(nextIt != endIt){
		curr = next;
		next = *nextIt;
		//length between two path points
		float len = Distance(curr, next);
		pathLen += len;
		//if need to draw node on this path line
		if(pathLen > nod_length * nodIndex){
			float l = abs(pathLen - len - nod_length * nodIndex);
			float k = l/len;
			while( k < 1 ){
				float x = curr.x + (next.x - curr.x) * k;
				float y = curr.y + (next.y - curr.y) * k;
				if(nodIndex <= body_length / nod_length){
					while(body_nodes.size() <= nodIndex){
						Body* previous = body_nodes[body_nodes.size() - 1];
						Body* cur;
						if(nodIndex > 1){
							cur = new Body(Point(0, 0));
						}else{
							cur = new Body(Point(0, 0), 20.f);
						}
						previous->SetNext(cur);
						body_nodes.push_back(cur);
					}
					Body* node = body_nodes[nodIndex];
					node->SetPosition(Point(x, y));
					node->Update(sec);
					if(node->NeedMore()){
						body_length += nod_length;
					}
				}
				nodIndex++;
				l += nod_length;
				k = l / len;
			}
		}
		//remove unused path
		if(pathLen > body_length){
			nextIt = body_path.erase(nextIt, body_path.end());
		}else{
			++nextIt;
		}
	}
}

void Snake::DrawBody(){
	for(Body* b : body_nodes){
		b->Draw();
	}
}

void Snake::Die(){
	punch->Play();
	dead = true;
	screen->SetState(GameState::GAMEOVER);
}