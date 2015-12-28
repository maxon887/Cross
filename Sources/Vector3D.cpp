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
#include "Vector3D.h"
#include <math.h>

Vector3D::Vector3D():
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Vector3D::Vector3D(float x, float y, float z):
	x(x),
	y(y),
	z(z)
{
}

Vector3D::Vector3D(Vector2D &vec, float z):
	x(vec.x),
	y(vec.y),
	z(z)
{
}

float Vector3D::Length(){
	return sqrt(x*x + y*y + z*z);
}

Vector3D& Vector3D::Normalize(){
	Vector3D result;
	float len = Length();
	result.x = x / len;
	result.y = y / len;
	result.z = z / len;
	return result;
}

Vector3D& Vector3D::Truncate(float len){
	if(this->Length() > len){
		Vector3D result;
		result = this->Normalize();
		result *= len;
		return result;
	}
	return *this;
}

float Vector3D::DotProduct(const Vector3D &v2){
	return this->x * v2.x + this->y * v2.y + this->z * v2.z;
}

Vector3D& Vector3D::CrossProduct(const Vector3D &v2){
	Vector3D result;
	result.x = this->y * v2.z - this->z * v2.y;
	result.y = this->z * v2.x - this->x * v2.z;
	result.z = this->x * v2.y - this->y * v2.x;
	return result;
}

Vector3D& Vector3D::operator+(const Vector3D &v2) const{
	return Vector3D(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

void Vector3D::operator+=(const Vector3D &v2){
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
}

Vector3D& Vector3D::operator-(const Vector3D &v2) const{
	return Vector3D(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

void Vector3D::operator-=(const Vector3D &v2){
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
}

Vector3D& Vector3D::operator*(const float v) const{
	return Vector3D(this->x * v, this->y * v, this->z * v);
}

void Vector3D::operator*=(const float v){
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

Vector3D& Vector3D::operator/(const float v) const{
	return Vector3D(this->x / v, this->y / v, this->z / v);
}

void Vector3D::operator/=(const float v){
	this->x /= v;
	this->y /= v;
	this->z /= v;
}

// ***************** General functions ********************
float Distance(const Vector3D &v1, const Vector3D &v2){
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) + 
				(v1.y - v2.y)*(v1.y - v2.y) +
				(v1.z - v2.z)*(v1.z - v2.z));
}

float DistanceSq(const Vector3D &v1, const Vector3D &v2){
	return  (v1.x - v2.x)*(v1.x - v2.x) +
			(v1.y - v2.y)*(v1.y - v2.y) +
			(v1.z - v2.z)*(v1.z - v2.z);
}