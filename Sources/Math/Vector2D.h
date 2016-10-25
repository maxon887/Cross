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

class Vector2D{
public:
	static float Dot(const Vector2D& left, const Vector2D& right);

	float x;
	float y;

	Vector2D();
	Vector2D(float value);
	Vector2D(float x, float y);

	float Length() const;
	Vector2D GetNormalized() const;
	Vector2D GetTruncated(float len) const;
	float* GetData();

	bool operator == (const Vector2D &v2) const;
	bool operator != (const Vector2D &v2) const;
	
	Vector2D operator + (const Vector2D &v2) const;
	Vector2D operator - (const Vector2D &v2) const;
	Vector2D operator * (float value) const;
	Vector2D operator / (float value) const;

	void operator += (const Vector2D &v2);
	void operator -= (const Vector2D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}