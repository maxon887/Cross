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
#include "Behaviour.h"
#include <algorithm> 

Behaviour::Behaviour(MovingEntity* owner){
	this->owner = owner;
}

Vector2D Behaviour::Seek(Vector2D targetPos){
	Vector2D desiredVelocity = targetPos - owner->pos;
	desiredVelocity = desiredVelocity.Normalize();
	desiredVelocity *= owner->max_speed;
	desiredVelocity -= owner->velocity;
	return desiredVelocity;
}

Vector2D Behaviour::Flee(Vector2D targetPos){
	const float panicDistanceSq = 100.f * 100.f;
	if(DistanceSq(owner->pos, targetPos) > panicDistanceSq){
		return Vector2D(0, 0);
	}else{
		Vector2D desiredVelocity = owner->pos - targetPos;
		desiredVelocity = desiredVelocity.Normalize();
		desiredVelocity *= owner->max_speed;
		desiredVelocity -= owner->velocity;
		return desiredVelocity;
	}
}

Vector2D Behaviour::Arrive(Vector2D targetPos, Deceleration dec){
	Vector2D toTarget = targetPos - owner->pos;
	float dist = toTarget.Length();
	if(dist > 0){
		const float decelerationTweaker = 1.3f;
		float speed = dist / (dec * decelerationTweaker);
		speed = min(speed, owner->max_speed);
		Vector2D desiredVelocity = toTarget * speed / dist;
		desiredVelocity -= owner->velocity;
		return desiredVelocity;
	}
	return Vector2D(0.f, 0.f);
}