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
#include "Snake.h"

const float		Cactus::growing_time = 4.f;
const float		Cactus::adult_time = 10.f;
const float		Cactus::old_time = 5.f;

Image*			Cactus::small_img = NULL;
Image*			Cactus::adult_img = NULL;
Image*			Cactus::old_img = NULL;
Audio*			Cactus::bith = NULL;
Audio*			Cactus::splash = NULL;

void Cactus::Init(){
	Cactus::small_img = graphics->LoadImage("Game/CactusSmall.png");
	Cactus::adult_img = graphics->LoadImage("Game/CactusAdult.png");
	Cactus::old_img = graphics->LoadImage("Game/CactusOld.png");
	if(game->IsSoundEnabled()){
		bith = new Audio("Game/CactusBirth.wav", false, false);
		splash = new Audio("Game/CactusSplash.wav", false, false);
	}
}

void Cactus::Release(){
	graphics->ReleaseImage(small_img);
	graphics->ReleaseImage(adult_img);
	graphics->ReleaseImage(old_img);

	delete bith;
	delete splash;
}

Cactus::Cactus(){
	scale_factor = 1.f;
	life_time = 0;
	shake_state = 3;
	lerp = 0;
	state = GROWING;
	shake_up = true;
}

Cactus::Cactus(Point pos):Collisioner(pos){
	scale_factor = 1.f;
	life_time = 0;
	shake_state = 3;
	lerp = 0;
	state = GROWING;
	shake_up = true;
}

float Cactus::GetRadius(){
	switch (state) {
	case GROWING:
		return 15.f;
	case ADULT:case Cactus::OLD:
		return 42.f;
	default:
		return 0;
	}
}

void Cactus::CollisionOccurred(Collisioner* obj){
	Snake* snake = dynamic_cast<Snake*>(obj);
	if(snake && state == GROWING){
		scale_factor = .5f;
		state = HIDING;
		life_time = 999;
		splash->Play();
	}
}

void Cactus::Update(float sec){
	static const float adultTime = growing_time + adult_time;
	static const float oldTime = growing_time + adult_time + old_time;
	Collisioner::Update(sec);
	life_time += sec;

	switch (state){
	case GROWING:{
		if(shake_up){
			scale_factor += sec * 2.f;
			if(scale_factor > 1.13)
				shake_up = false;
		}else{
			scale_factor -= sec * 2.f;
			if(scale_factor < 1)
				shake_up = true;
		}
		if(life_time > growing_time){
			state = ADULT;
			bith->Play();
		}
		}break;
	case ADULT:{
		if(shake_up){
			scale_factor += sec * shake_state;
			if(scale_factor > 1.13)
				shake_up = false;
		}else{
			scale_factor -= sec * shake_state;
			if(scale_factor < 1)
				shake_up = true;
		}
		if(lerp < 1)
			lerp += 0.13f * sec;
		else 
			lerp = 1;
		shake_state = Lerp(shake_state, 0, lerp);
		if(life_time > adultTime){
			graphics->ScaleImage(old_img, game->GetScaleFactor() * 0.5f * scale_factor);
			state = OLD;
		}
		}break;
		break;
	case OLD:
		if(life_time > oldTime){
			state = HIDING;
		}
		break;
	case HIDING:
		scale_factor -= sec * 2.f;
		if(scale_factor < 0){
			state = EMPTY;
		}
		break;
	case EMPTY:
		break;
	default:
		break;
	}
}

void Cactus::Draw(){
	switch(state){
	case GROWING:
		graphics->ScaleImage(small_img, game->GetScaleFactor() * 0.2f * scale_factor);
		graphics->DrawImage(GetPosition(), small_img);
		break;
	case ADULT:
		graphics->ScaleImage(adult_img, game->GetScaleFactor() * 0.5f * scale_factor);
		graphics->DrawImage(GetPosition(), adult_img);
		break;
	case OLD:case HIDING:
		graphics->ScaleImage(old_img, game->GetScaleFactor() * 0.5f * scale_factor);
		graphics->DrawImage(GetPosition(), old_img);
		break;
	case EMPTY:
		break;
	default:
		break;
	}
	graphics->DrawCircle(GetPosition(), GetRadius(), 1, 0, 0);
}

bool Cactus::IsDead(){
	if(state == EMPTY)
		return true;
	else
		return false;
}

bool Cactus::Dangerous(){
	if(state == ADULT || state == OLD)
		return true;
	else 
		return false;
}