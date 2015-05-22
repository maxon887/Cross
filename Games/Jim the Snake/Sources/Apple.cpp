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
	
#include "Apple.h"

const float Apple::radius = 12.f;
Graphics*	Apple::graphics = NULL;
Game*		Apple::game = NULL;
Image*		Apple::fresh_img = NULL;
Image*		Apple::rot_img = NULL;
Image*		Apple::dead_img = NULL;

void Apple::Init(Game* game){
	Apple::game = game;
	Apple::graphics = game->graphics;
	Apple::fresh_img = graphics->LoadImage("Game/AppleFresh.png");
	Apple::rot_img = graphics->LoadImage("Game/AppleRot.png");
	Apple::dead_img = graphics->LoadImage("Game/AppleDead.png");
}

Apple::Apple(){
	if(game == NULL)
		throw "Class Apple needs to be initialized staticly";
	this->angle = (float)(rand()%360);
	life_time = 15;
}

void Apple::Draw(){
	graphics->Rotate(current_img, angle);
	graphics->DrawImage(pos, current_img);
}

void Apple::SetPosition(PointX pos){
	this->pos = pos;
}

PointX Apple::GetPosition(){
	return pos;
}

float Apple::GetRadius(){
	return radius;
}

void Apple::Update(float sec){
	life_time -= sec;
	if(life_time < 4) {
		state = AppleState::DEAD;
		current_img = dead_img;
	}
	if(life_time > 4 && life_time < 8) {
		state = AppleState::ROT;
		current_img = rot_img;
	}
	if(life_time > 8) {
		state = AppleState::FRESH;
		current_img = fresh_img;
	}
}

float Apple::GetLifeTime(){
	return life_time;
}

AppleState Apple::GetState(){
	return state;
}

void Apple::SetLifeTime(float lifeTime){
	life_time = lifeTime;
}