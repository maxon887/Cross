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
	
#include "Cactus.h"
#include "Misc.h"

Image*		Cactus::small_img = NULL;
Image*		Cactus::adult_img = NULL;
Image*		Cactus::old_img = NULL;

void Cactus::Init(){
	Cactus::small_img = graphics->LoadImage("Game/CactusSmall.png");
	Cactus::adult_img = graphics->LoadImage("Game/CactusAdult.png");
	Cactus::old_img = graphics->LoadImage("Game/CactusOld.png");
}

void Cactus::Release(){
	graphics->ReleaseImage(small_img);
	graphics->ReleaseImage(adult_img);
	graphics->ReleaseImage(old_img);
}

Cactus::Cactus(){
	scale_factor = 1.f;
	life_time = 0;
	shake_state = 3;
	lerp = 0;
	state = GROWING;
	shake_up = true;
}

float Cactus::GetRadius(){
	return 12.f;
}

void Cactus::Update(float sec){
	life_time += sec;
	switch (state)
	{
	case GROWING:{
		//static bool up = true;
		if(shake_up){
			scale_factor += sec * 2.f;
			if(scale_factor > 1.13)
				shake_up = false;
		}else{
			scale_factor -= sec * 2.f;
			if(scale_factor < 1)
				shake_up = true;
		}
		game->launcher->LogIt(scale_factor);
		graphics->ScaleImage(small_img, game->GetScaleFactor() * 0.2f * scale_factor);
		}break;
	case ADULT:{
		//static bool up = true;
		if(shake_up){
			scale_factor += sec * shake_state;
			if(scale_factor > 1.13)
				shake_up = false;
		}else{
			scale_factor -= sec * shake_state;
			if(scale_factor < 1)
				shake_up = true;
		}
		graphics->ScaleImage(adult_img, game->GetScaleFactor() * 0.5f * scale_factor);
		if(lerp < 1)
			lerp += 0.13f * sec;
		else 
			lerp = 1;
		shake_state = Lerp(shake_state, 0, lerp);
		}break;
		break;
	case OLD:
		break;
	case HIDING:
		scale_factor -= sec * 2.f;
		graphics->ScaleImage(old_img, game->GetScaleFactor() * 0.5f * scale_factor);
		break;
	case EMPTY:
		break;
	default:
		break;
	}
	if(life_time >= 3){
		state = ADULT;
	}
	if(life_time >= 10 && state != OLD){
		graphics->ScaleImage(old_img, game->GetScaleFactor() * 0.5f * scale_factor);
		state = OLD;
	}
	if(life_time >= 14 && state != HIDING){
		state = HIDING;
	}
	if(scale_factor < 0){
		state = EMPTY;
	}
}

void Cactus::Draw(){
	graphics->DrawCircle(GetPosition(), GetRadius(), 1, 0, 0);
	switch (state) {
	case Cactus::GROWING:
		graphics->DrawImage(GetPosition(), small_img);
		break;
	case Cactus::ADULT:
		graphics->DrawImage(GetPosition(), adult_img);
		break;
	case Cactus::OLD:case Cactus::HIDING:
		graphics->DrawImage(GetPosition(), old_img);
		break;
	case Cactus::EMPTY:
		break;
	default:
		break;
	}
}

bool Cactus::IsDead(){
	if(state == Cactus::EMPTY)
		return true;
	else
		return false;
}