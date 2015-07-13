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

#include "Body.h"
	
Image*			Body::img			= NULL;

Body::Body(Point pos){
	big = false;
	need_more = false;
	next = NULL;
	time_left = -1;
}

float Body::GetRadius(){
	return 26.f;
}

void Body::Update(float sec){
	if(time_left > 0){
		time_left -= sec;
		if(time_left < 0){
			big = false;
			if(next == NULL){
				need_more = true;
				return;
			}else{
				next->SetBig();
			}
		}
	}
	//return false;
}

void Body::Draw(){
	if(big){
		graphics->ScaleImage(img, game->GetScaleFactor() * 1.2f);
	}else{
		graphics->ScaleImage(img, game->GetScaleFactor());
	}
	graphics->DrawImage(GetPosition(), img);
}

void Body::SetBig(){
	big = true;
	time_left = 0.3f;
}

void Body::SetNext(Body* next){
	this->next = next;
}

bool Body::NeedMore(){
	if(need_more){
		need_more = false;
		return true;
	}else{
		return false;
	}
}