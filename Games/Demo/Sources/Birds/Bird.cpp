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
#include "Bird.h"

Bird::Bird(float mass, float maxSpeed, float maxForce, float maxTurnRate):
	MovingEntity(mass, maxSpeed, maxForce, maxTurnRate){
	behaviour = new Behaviour(this);
}

void Bird::Update(float sec, Vector2D targetPos){
	Vector2D steeringForce = behaviour->Seek(targetPos);
	Vector2D acceleration = steeringForce / mass;
	velocity += acceleration * sec;
	velocity.Truncate(max_speed);
	pos += velocity * sec;
}

void Bird::Draw(){
	Point p1;
	p1.x = pos.x;
	p1.y = pos.y;
	Point p2;
	p2.x = pos.x + velocity.x;
	p2.y = pos.y + velocity.y;
	Vector2D perpVector1;
	perpVector1.x = pos.x + velocity.x / 3;
	perpVector1.y = pos.y - velocity.y / 3;
	Vector2D perpVector2;
	perpVector2.x = pos.x - velocity.x / 3;
	perpVector2.y = pos.y + velocity.y / 3;

	graphics->DrawLine(p2, Point(perpVector1.x, perpVector1.y), Color::Blue);
	graphics->DrawLine(p2, Point(perpVector2.x, perpVector2.y), Color::Blue);
	graphics->DrawLine(Point(perpVector1.x, perpVector1.y), Point(perpVector2.x, perpVector2.y), Color::Blue);
}