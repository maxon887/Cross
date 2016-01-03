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
#include "Vector2D.h"

namespace cross{

class Vector3D{
public:
	float x;
	float y;
	float z;

	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(Vector2D &vec, float z);

	float Length();
	Vector3D Normalize();
	Vector3D Truncate(float len);
	float DotProduct(const Vector3D &v2);
	Vector3D CrossProduct(const Vector3D &v2);

	Vector3D operator + (const Vector3D &v2) const;
	void operator += (const Vector3D &v2);
	Vector3D operator - (const Vector3D &v2) const;
	void operator -= (const Vector3D &v2);
	Vector3D operator * (const float value) const;
	void operator *= (const float value);
	Vector3D operator / (const float value) const;
	void operator /= (const float value);
};

float Distance(const Vector3D &v1, const Vector3D &v2);
float DistanceSq(const Vector3D &v1, const Vector3D &v2);

}