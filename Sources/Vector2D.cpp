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

using namespace cross;

Vector2D::Vector2D():
	x(0.0f),
	y(0.0f)
{
}

Vector2D::Vector2D(float x, float y):
	x(x),
	y(y)
{
}

float Vector2D::Length(){
	return sqrt(x*x + y*y);
}

Vector2D Vector2D::Normalize(){
	Vector2D result;
	float len = Length();
	result.x  = x / len;
	result.y  = y / len;
	return result;
}

Vector2D Vector2D::Truncate(float len){
	if(this->Length() > len){
		Vector2D result;
		result = this->Normalize();
		result *= len;
		return result;
	}
	return *this;
}

float Vector2D::DotProduct(const Vector2D &v2){
	return this->x * v2.x + this->y * v2.y;
}

Vector2D Vector2D::operator+(const Vector2D &v2) const{
	return Vector2D(this->x + v2.x, this->y + v2.y);
}

void Vector2D::operator+=(const Vector2D &v2){
	this->x += v2.x;
	this->y += v2.y;
}

Vector2D Vector2D::operator-(const Vector2D &v2) const{
	return Vector2D(this->x - v2.x, this->y - v2.y);
}

void Vector2D::operator-=(const Vector2D &v2){
	this->x -= v2.x;
	this->y -= v2.y;
}

Vector2D Vector2D::operator*(const float v) const{
	return Vector2D(this->x * v, this->y * v);
}

void Vector2D::operator*=(const float v){
	this->x *= v;
	this->y *= v;
}

Vector2D Vector2D::operator/(const float v) const{
	return Vector2D(this->x / v, this->y / v);
}

void Vector2D::operator/=(const float v){
	this->x /= v;
	this->y /= v;
}

bool Vector2D::operator==(const Vector2D &v2) const{
	if(this->x == v2.x && this->y && v2.y) {
		return true;
	} else {
		return false;
	}
}

bool Vector2D::operator!=(const Vector2D &v2) const{
	return !((*this) == v2);
}