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
#include "Misc.h"

static float sign(cross::Vector2D p1, cross::Vector2D p2, cross::Vector2D p3){
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

int cross::Random(int max){
	return rand() % max;
}

int cross::Random(int min, int max){
	return (min) + rand() % (max - min);
}

float cross::Random(float max){
	return rand() / (RAND_MAX / max);
}

float cross::Random(float min, float max){
	return (min)+rand() / (RAND_MAX / (max - min));
}

bool cross::PointInRect(Vector2D p, Rect rect){
	return  p.x > rect.x &&
			p.x < rect.x + rect.width &&
			p.y > rect.y &&
			p.y < rect.y + rect.height;
}

bool cross::PointInTriangle(Vector2D pt, Vector2D v1, Vector2D v2, Vector2D v3){
	bool b1, b2, b3;
    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;
    return ((b1 == b2) && (b2 == b3));
}

bool cross::PointInCircle(Vector2D p, Vector2D center, float radius){
	float distance = Distance(p, center);
	if(distance < radius)
		return true;
	else return false;
}

bool cross::CircleOnCollision(Vector2D p1, float r1, Vector2D p2, float r2) {
	float distance = Distance(p1, p2);
	if(distance < (r1 + r2))
		return true;
	else return false;
}

float cross::Distance(const Vector2D &v1, const Vector2D &v2){
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y));
}

float cross::DistanceSq(const Vector2D &v1, const Vector2D &v2){
	return (v1.x - v2.x)*(v1.x - v2.x) + (v1.y - v2.y)*(v1.y - v2.y);
}

float cross::Distance(const Vector3D &v1, const Vector3D &v2){
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) +
		(v1.y - v2.y)*(v1.y - v2.y) +
		(v1.z - v2.z)*(v1.z - v2.z));
}

float cross::DistanceSq(const Vector3D &v1, const Vector3D &v2){
	return  (v1.x - v2.x)*(v1.x - v2.x) +
		(v1.y - v2.y)*(v1.y - v2.y) +
		(v1.z - v2.z)*(v1.z - v2.z);
}