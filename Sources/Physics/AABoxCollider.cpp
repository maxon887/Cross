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
#include "AABoxCollider.h"

using namespace cross;

AABoxCollider::AABoxCollider(Vector3D begin, Vector3D end) :
	begin(begin),
	end(end)
{ }

Collision AABoxCollider::OnCollision(AABoxCollider* other){
	Vector3D dist1 = this->begin - other->end;
	Vector3D dist2 = other->begin - this->end;

	Vector3D max(0.f);
	max.x = dist1.x > dist2.x ? dist1.x : dist2.x;
	max.y = dist1.y > dist2.y ? dist1.y : dist2.y;
	max.z = dist1.z > dist2.z ? dist1.z : dist2.z;

	float maxVal = 0.f;
	if(max.x > max.y){
		if(max.x > max.z){
			maxVal = max.x;
		}else{
			maxVal = max.z;
		}
	}else{
		if(max.y > max.z){
			maxVal = max.y;
		}else{
			maxVal = max.z;
		}
	}

	return Collision(maxVal < 0, maxVal);
}

Vector3D AABoxCollider::GetBegin() const{
	return begin;
}

Vector3D AABoxCollider::GetEnd() const {
	return end;
}