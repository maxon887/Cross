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

namespace cross{

class Vector2D;
class Vector4D;

class Vector3D{
public:
	float x;
	float y;
	float z;

	Vector3D();
	Vector3D(float value);
	Vector3D(float x, float y, float z);
	Vector3D(const Vector2D &vec, float z);
	Vector3D(const Vector4D &vec);

	float Length() const;
	Vector3D Normalize() const;
	Vector3D Truncate(float len) const;
	float DotProduct(const Vector3D &v2) const;
	Vector3D CrossProduct(const Vector3D &v2) const;
	float* GetData();

	bool operator == (const Vector3D &v2) const;
	bool operator != (const Vector3D &v2) const;
	Vector3D operator + (const Vector3D &v2) const;
	void operator += (const Vector3D &v2);
	Vector3D operator - (const Vector3D &v2) const;
	void operator -= (const Vector3D &v2);
	Vector3D operator * (float value) const;
	void operator *= (float value);
	Vector3D operator / (float value) const;
	void operator /= (float value);
};

}