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
#include <math.h>

static float sign(cross::Point p1, cross::Point p2, cross::Point p3){
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool cross::PointInRect(Point p, Rect rect){
	return  p.x > rect.x &&
			p.x < rect.x + rect.width &&
			p.y > rect.y &&
			p.y < rect.y + rect.height;
}

bool cross::CircleOnCollision(Point p1, float r1, Point p2, float r2) {
	float distance = Distance(p1, p2);
	if(distance < (r1 + r2))
		return true;
	else return false;
}

float cross::Lerp(float v0, float v1, float t) {
	return (1-t)*v0 + t*v1;
}

bool cross::PointInTriangle (Point pt, Point v1, Point v2, Point v3){
	bool b1, b2, b3;

    b1 = sign(pt, v1, v2) < 0.0f;
    b2 = sign(pt, v2, v3) < 0.0f;
    b3 = sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

float cross::Distance(Point p1, Point p2){
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

float cross::Angle(Point first, Point second){
	double tangens = (first.x - second.x) / (first.y - second.y);
	double angle = atan(tangens);
	angle = angle * 180. / PI;
	if(first.y > second.y) {
		angle += 90;
	} else {
		angle -= 90;
	}
	return (float)angle;
}
