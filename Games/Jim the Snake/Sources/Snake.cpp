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

const float Snake::face_radius = 26.f;
const float Snake::body_radius = 26.f;
const float Snake::speedV = 240.f;
const float Snake::speedW = speedV / 1.33f;
const float Snake::nod_length = 50.f;
const float Snake::star_speedW = 2.f;

Snake::Snake(Game* game){
	body_length = 150.f;
	this->graphics = game->graphics;
}

bool Snake::OnCollision(PointX center, float radius){
	for(PointX node : body_nodes){
		if(CircleOnCollision(node, body_radius, center, radius))
			return true;
	}
	if(CircleOnCollision(face_pos, face_radius, center, radius))
		return true;
	return false;
}

void Snake::EatableNear(Eatable* eatable){
	if(CircleOnCollision(face_pos, face_radius, eatable->GetPosition(), eatable->GetRadius() + 60.f)) {
		if(!face_bottom_anim->IsRunning()) {
			face_bottom_anim->Start();
			face_top_anim->Start();
			if(eat_snd != NULL)
				eat_snd->Play();
		}
		if(CircleOnCollision(face_pos, face_radius, eatable->GetPosition(), 1.f)) {
			apple_time_left = 0.025f;
			near_eatable = eatable;
		}
	}
}