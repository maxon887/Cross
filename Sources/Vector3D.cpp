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
#include "Vector2D.h"
#include "Vector4D.h"
#include <math.h>

using namespace cross;

const Vector3D Vector3D::Zero(0.f, 0.f, 0.f);
const Vector3D Vector3D::Up(0.f, 1.f, 0.f);

Vector3D::Vector3D():
	x(0.0f),
	y(0.0f),
	z(0.0f)
{ }

Vector3D::Vector3D(float value):
	x(value),
	y(value),
	z(value)
{ }

Vector3D::Vector3D(float x, float y, float z):
	x(x),
	y(y),
	z(z)
{ }

Vector3D::Vector3D(const Vector2D &vec, float z):
	x(vec.x),
	y(vec.y),
	z(z)
{ }

Vector3D::Vector3D(const Vector4D &vec):
	x(vec.x),
	y(vec.y),
	z(vec.z)
{ }

float Vector3D::Length() const{
	return (float)sqrt(x*x + y*y + z*z);
}

Vector3D Vector3D::Normalize() const{
	Vector3D result;
	float len = Length();
	result.x = x / len;
	result.y = y / len;
	result.z = z / len;
	return result;
}

bool Vector3D::IsNormalized() const{
	float len = Length();
	return len > 0.999 && len < 1.001;
}

Vector3D Vector3D::Truncate(float len) const{
	if(this->Length() > len){
		Vector3D result;
		result = this->Normalize();
		result *= len;
		return result;
	}
	return *this;
}

float Vector3D::DotProduct(const Vector3D &v2) const{
	return this->x * v2.x + this->y * v2.y + this->z * v2.z;
}

Vector3D Vector3D::CrossProduct(const Vector3D &v2) const{
	Vector3D result;
	result.x = this->y * v2.z - this->z * v2.y;
	result.y = this->z * v2.x - this->x * v2.z;
	result.z = this->x * v2.y - this->y * v2.x;
	return result;
}

float* Vector3D::GetData(){
	return &x;
}

bool Vector3D::operator==(const Vector3D &v2) const{
	if(this->x == v2.x && this->y == v2.y && this->z == v2.z){
		return true;
	}else{
		return false;
	}
}

bool Vector3D::operator!=(const Vector3D &v2) const{
	return !(*this == v2);
}

Vector3D Vector3D::operator+(const Vector3D &v2) const{
	return Vector3D(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

void Vector3D::operator+=(const Vector3D &v2){
	this->x += v2.x;
	this->y += v2.y;
	this->z += v2.z;
}

Vector3D Vector3D::operator-(const Vector3D &v2) const{
	return Vector3D(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

void Vector3D::operator-=(const Vector3D &v2){
	this->x -= v2.x;
	this->y -= v2.y;
	this->z -= v2.z;
}

Vector3D Vector3D::operator*(float v) const{
	return Vector3D(this->x * v, this->y * v, this->z * v);
}

void Vector3D::operator*=(float v){
	this->x *= v;
	this->y *= v;
	this->z *= v;
}

Vector3D Vector3D::operator/(float v) const{
	return Vector3D(this->x / v, this->y / v, this->z / v);
}

void Vector3D::operator/=(float v){
	this->x /= v;
	this->y /= v;
	this->z /= v;
}