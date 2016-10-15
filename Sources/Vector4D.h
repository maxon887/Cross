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

class Vector3D;

class Vector4D{
public:
	float x;
	float y;
	float z;
	float w;

	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(const Vector3D& vec, float w);

	Vector4D operator + (const Vector4D &v2) const;
	Vector4D operator - (const Vector4D &v2) const;
	Vector4D operator * (float value) const;
	Vector4D operator / (float value) const;

	void operator += (const Vector4D &v2);
	void operator -= (const Vector4D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}