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
	Vector2D():x(.0f), y(.0f){};
	Vector2D(float x, float y):x(x), y(y){};
	float Length();
	void Normalize();
	float DotProduct(const Vector2D &v2);
	Vector2D CrossProduct(const Vector2D &v2);
	
	Vector2D operator + (const Vector2D &v2);
	void operator += (const Vector2D &v2);
private:
	float x;
	float y;
};