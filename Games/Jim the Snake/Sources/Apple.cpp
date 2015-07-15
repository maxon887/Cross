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
#include <stdlib.h>

Image*		Apple::fresh_img = NULL;
Image*		Apple::rot_img = NULL;
Image*		Apple::dead_img = NULL;
int			Apple::count = 0;

void Apple::Init(){
	Apple::fresh_img = graphics->LoadImage("Game/AppleFresh.png");
	Apple::rot_img = graphics->LoadImage("Game/AppleRot.png");
	Apple::dead_img = graphics->LoadImage("Game/AppleDead.png");
}

void Apple::Release(){
	graphics->ReleaseImage(fresh_img);
	graphics->ReleaseImage(rot_img);
	graphics->ReleaseImage(dead_img);
}

Apple::Apple(){
	this->angle = (float)(rand()%360);
	life_time = 15;
	state = AppleState::FRESH;
	count++;
}

Apple::~Apple(){
	count--;
}

int Apple::Eat(){
	switch (state){
	case AppleState::FRESH:
		state = AppleState::EMPTY;
		return 2;
	case AppleState::ROT:
		state = AppleState::EMPTY;
		return 1;
	default:
		state = AppleState::EMPTY;
		return 0;
	}
}

bool Apple::Eaten(){
	if(state == AppleState::EMPTY)
		return true;
	else
		return false;
}

bool Apple::Useless(){
	if(state == AppleState::EMPTY)
		return true;
	else
		return false;
}

void Apple::Draw(){
	Image* current = NULL;
	switch (state)
	{
	case AppleState::FRESH:
		current = fresh_img;
		break;
	case AppleState::ROT:
		current = rot_img;
		break;
	case AppleState::DEAD:
		current = dead_img;
		break;
	case AppleState::EMPTY:
		return;
	default:
		throw string("Unexpected apple behavior");
	}
	graphics->Rotate(current, angle);
	graphics->DrawImage(GetPosition(), current);
	graphics->DrawCircle(GetPosition(), GetRadius(), Color::Green);
}

float Apple::GetRadius(){
	return 12.f;
}

void Apple::Update(float sec){
	Collisioner::Update(sec);
	life_time -= sec;
	if(life_time < 0){
		state = AppleState::EMPTY;
		return;
	}
	if(life_time < 4) {
		state = AppleState::DEAD;
		return;
	}
	if(life_time < 8) {
		state = AppleState::ROT;
		return;
	}
    state = AppleState::FRESH;
}
