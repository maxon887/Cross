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
#include "Cross.h"

#define PI (3.141592653589793f)

namespace cross {

int Random(int max);
int Random(int min, int max);
float Random(float max);
float Random(float min, float max);
bool PointInRect(Vector2D p, Rect rect);
bool PointInTriangle(Vector2D pt, Vector2D v1, Vector2D v2, Vector2D v3);
bool PointInCircle(Vector2D p, Vector2D center, float radius);
bool CircleOnCollision(Vector2D p1, float r1, Vector2D p2, float r2);
float Distance(const Vector2D &v1, const Vector2D &v2);
float DistanceSq(const Vector2D &v1, const Vector2D &v2);
float Distance(const Vector3D &v1, const Vector3D &v2);
float DistanceSq(const Vector3D &v1, const Vector3D &v2);

}