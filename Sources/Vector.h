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

class Vector{
public:
	float x;
	float y;
	float z;

	Vector() :x(0.0f), y(0.0f), z(0.0f) { };
	Vector(float x, float y) :x(x), y(y), z(0.0f) { };
	Vector(float x, float y, float z) :x(x), y(y), z(z) { };
	float Length();
	Vector Normalize();
	Vector Truncate(float len);
	float DotProduct(const Vector &v2);
	Vector CrossProduct(const Vector &v2);
	
	Vector operator + (const Vector &v2) const;
	void operator += (const Vector &v2);
	Vector operator - (const Vector &v2) const;
	void operator -= (const Vector &v2);
	Vector operator * (const float value) const;
	void operator *= (const float value);
	Vector operator / (const float value) const;
	void operator /= (const float value);
};

float Distance(const Vector &v1, const Vector &v2);
float DistanceSq(const Vector &v1, const Vector &v2);