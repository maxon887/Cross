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
#include "Vector.h"

#include <math.h>

float Vector::Length(){
	return sqrt(x*x + y*y);
}

Vector Vector::Normalize(){
	Vector result;
	float len = Length();
	result.x = x / len;
	result.y = y / len;
	result.z = z / len;
	return result;
}

Vector Vector::Truncate(float len){
	if(this->Length() > len){
		Vector result;
		result = this->Normalize();
		result *= len;
		return result;
	}
	return *this;
}

float Vector::DotProduct(const Vector &v2){
	return this->x * v2.x + this->y * v2.y + this->z * v2.z;
}

Vector Vector::CrossProduct(const Vector &v2){
	Vector result;
	result.x = this->y * v2.z - this->z * v2.y;
	result.y = this->z * v2.x - this->x * v2.z;
	result.z = this->x * v2.y - this->y * v2.x;
	return result;
}

Vector Vector::operator+(const Vector &v2) const{
	return Vector(this->x + v2.x, this->y + v2.y, + this->z + v2.z);
}

void Vector::operator+=(const Vector &v2){
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
}

Vector Vector::operator-(const Vector &v2) const{
	return Vector(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

void Vector::operator-=(const Vector &v2){
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
}

Vector Vector::operator*(const float v) const{
	return Vector(this->x * v, this->y * v, this->z * v);
}

void Vector::operator*=(const float v){
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

Vector Vector::operator/(const float v) const{
	return Vector(this->x / v, this->y / v, this->z / v);
}

void Vector::operator/=(const float v){
	this->x /= v;
	this->y /= v;
	this->z /= v;
}

// ***************** General functions ********************
float Distance(const Vector &v1, const Vector &v2){
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) + 
				(v1.y - v2.y)*(v1.y - v2.y) +
				(v1.z - v2.z)*(v1.z - v2.z));
}

float DistanceSq(const Vector &v1, const Vector &v2){
	return	(v1.x - v2.x)*(v1.x - v2.x) + 
			(v1.y - v2.y)*(v1.y - v2.y) +
			(v1.z - v2.z)*(v1.z - v2.z);
}