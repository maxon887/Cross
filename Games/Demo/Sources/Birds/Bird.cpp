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
#include "Texter.h"
#include "Utils\Debuger.h"

Bird::Bird(float mass, float maxSpeed, float maxForce, float maxTurnRate):
	MovingEntity(mass, maxSpeed, maxForce, maxTurnRate){
	behaviour = new Behaviour(this);
}

void Bird::Update(float sec, Vector2D targetPos){
	Vector2D steeringForce = behaviour->Arrive(targetPos, Behaviour::FAST);
	Vector2D acceleration = steeringForce / mass;
	velocity += acceleration * sec;
	velocity = velocity.Truncate(max_speed);
	pos += velocity * sec;
	if(velocity.Length() >= 5.f){
		heading = velocity.Normalize();
	}

	Texter* texter = debuger->GetTexter();
	string speedMsg = string("Speed - " + to_string(velocity.Length()));
	texter->DrawText(Point(0, game->GetHeight() - texter->GetHeight()), speedMsg);
	string forceMsg = string("Force - " + to_string(steeringForce.Length()));
	texter->DrawText(Point(0, game->GetHeight() - texter->GetHeight() * 2), forceMsg);
	string accMsg = string("Accel - " + to_string(acceleration.Length()));
	texter->DrawText(Point(0, game->GetHeight() - texter->GetHeight() * 3), accMsg);
	graphics->DrawLine(Point(pos.x, pos.y), Point(pos.x + acceleration.x, pos.y + acceleration.y), Color::Yellow);
	graphics->DrawLine(Point(pos.x, pos.y), Point(pos.x + steeringForce.x, pos.y + steeringForce.y), Color::Red);
	graphics->DrawLine(Point(pos.x, pos.y), Point(pos.x + velocity.x, pos.y + velocity.y), Color::Green);
}

void Bird::Draw(){
	Vector2D normVelocity = heading;
	Vector2D perpVector1(-normVelocity.y, normVelocity.x);
	Vector2D perpVector2(normVelocity.y, -normVelocity.x);
	perpVector1 *= 10;
	perpVector2 *= 10;
	normVelocity *= 30;
	Point p1;
	p1.x = pos.x + normVelocity.x;
	p1.y = pos.y + normVelocity.y;
	perpVector1 += pos;
	perpVector2 += pos;

	graphics->DrawLine(p1, Point(perpVector1.x, perpVector1.y), Color::Blue);
	graphics->DrawLine(p1, Point(perpVector2.x, perpVector2.y), Color::Blue);
	graphics->DrawLine(Point(perpVector1.x, perpVector1.y), Point(perpVector2.x, perpVector2.y), Color::Blue);
}