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

class Vector2D{
public:
	float x;
	float y;

	Vector2D():x(.0f), y(.0f){};
	Vector2D(float x, float y):x(x), y(y){};
	float Length();
	Vector2D Normalize();
	Vector2D Truncate(float len);
	float DotProduct(const Vector2D &v2);
	
	Vector2D operator + (const Vector2D &v2) const;
	void operator += (const Vector2D &v2);
	Vector2D operator - (const Vector2D &v2) const;
	void operator -= (const Vector2D &v2);
	Vector2D operator * (const float value) const;
	void operator *= (const float value);
	Vector2D operator / (const float value) const;
	void operator /= (const float value);
};

float Distance(const Vector2D &v1, const Vector2D &v2);
float DistanceSq(const Vector2D &v1, const Vector2D &v2);