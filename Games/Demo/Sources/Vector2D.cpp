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
	
#include "Vector2D.h"

#include <math.h>

float Vector2D::Length(){
	return sqrt(x*x + y*y);
}

void Vector2D::Normalize(){
	float len = Length();
	x /= len;
	y /= len;
}

float Vector2D::DotProduct(const Vector2D &v2){
	return this->x * v2.x + this->y * v2.y;
}

Vector2D Vector2D::operator+(const Vector2D &v2){
	return Vector2D(this->x + v2.x, this->y + v2.y);
}

void Vector2D::operator+=(const Vector2D &v2){
	this->x += v2.x;
	this->y += v2.y;
}