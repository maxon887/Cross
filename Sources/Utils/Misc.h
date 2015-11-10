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
	
#pragma once

#include "Point.h"
#include "Rect.h"

namespace cross {

#define PI (3.141592653589793f)

bool PointInRect(Point p, Rect rect);
bool PointInTriangle(Point pt, Point v1, Point v2, Point v3);
bool PointInCircle(Point p, Point center, float radius);
bool CircleOnCollision(Point p1, float r1, Point p2, float r2);
float Lerp(float v0, float v1, float t);
float Distance(const Point &p1, const Point &p2);
float Angle(const Point &first, const Point &second);

}