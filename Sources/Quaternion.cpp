/*	Copyright � 2015 Lukyanau Maksim

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
#include "Quaternion.h"
#include "Cross.h"

#include <math.h>

using namespace cross;

Quaternion::Quaternion():
	x(0.f),
	y(0.f),
	z(0.f),
	w(0.f)
{ }

Quaternion::Quaternion(const Vector3D& axis, float angle){ 
	Vector3D normV = axis.Normalize();
	float halfAngle = angle * 0.5f;
	float cosA = cos(halfAngle / 180.f * PI);
	float sinA = sin(halfAngle / 180.f * PI);
	this->x = normV.x * sinA;
	this->y = normV.y * sinA;
	this->z = normV.z * sinA;
	this->w = cosA;
}

Quaternion::Quaternion(const Matrix& m){
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2]; // trace of m.martix
	if (tr > 0.0f){     // if trace positive than "w" is biggest com.mponent
		this->x = m.m[1][2] - m.m[2][1];
		this->y = m.m[2][0] - m.m[0][2];
		this->z = m.m[0][1] - m.m[1][0];
		this->w = tr + 1.0f;
	}else                 // Som.me of vector com.mponents is bigger
	if( (m.m[0][0] > m.m[1][1] ) && ( m.m[0][0] > m.m[2][2]) ) {
		this->x = 1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2];
		this->y = m.m[1][0] + m.m[0][1];
		this->z = m.m[2][0] + m.m[0][2];
		this->w = m.m[1][2] - m.m[2][1];
	}else 
	if ( m.m[1][1] > m.m[2][2] ){
		this->x = m.m[1][0] + m.m[0][1];
		this->y = 1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2];
		this->z = m.m[2][1] + m.m[1][2];
		this->w = m.m[2][0] - m.m[0][2];
	}else{
		this->x = m.m[2][0] + m.m[0][2];
		this->y = m.m[2][1] + m.m[1][2];
		this->z = 1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1];
		this->w = m.m[0][1] - m.m[1][0];
	}
}

void Quaternion::Scale(float scale){
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	this->w *= scale;
}

float Quaternion::Norm() const{
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}

float Quaternion::Length() const{
	return sqrt(this->Norm());
}

Quaternion Quaternion::Conjugate() const{
	Quaternion result = *this;
	result.x *= -1;
	result.y *= -1;
	result.z *= -1;
	return result;
}

float Quaternion::DotProduct(const Quaternion& q) const{
	return this->x * q.x + this->y * q.y + this->z * q.z + this->w * q.w;
}

Matrix Quaternion::GetMatrix() const{
	Matrix m = Matrix::CreateIdentity();
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    float s  = 2.0f / this->Norm();  // 4 mul 3 add 1 div
    x2 = this->x * s;    y2 = this->y * s;    z2 = this->z * s;
    xx = this->x * x2;   xy = this->x * y2;   xz = this->x * z2;
    yy = this->y * y2;   yz = this->y * z2;   zz = this->z * z2;
    wx = w * x2;   wy = w * y2;   wz = w * z2;

    m.m[0][0] = 1.0f - (yy + zz);
    m.m[1][0] = xy - wz;
    m.m[2][0] = xz + wy;

    m.m[0][1] = xy + wz;
    m.m[1][1] = 1.0f - (xx + zz);
    m.m[2][1] = yz - wx;

    m.m[0][2] = xz - wy;
    m.m[1][2] = yz + wx;
    m.m[2][2] = 1.0f - (xx + yy);
	
	return m;
}

Quaternion Quaternion::operator+(const Quaternion& q) const{
	Quaternion result;
	result.x = this->x + q.x;
	result.y = this->y + q.y;
	result.x = this->x + q.z;
	result.w = this->w + q.w;
	return result;
}

Quaternion Quaternion::operator-(const Quaternion& q) const{
	Quaternion result;
	result.x = this->x - q.x;
	result.y = this->y - q.y;
	result.x = this->x - q.z;
	result.w = this->w - q.w;
	return result;
}

Quaternion Quaternion::operator*(const Quaternion& q) const{
	Quaternion result;

    result.x = this->x * q.w + this->w * q.x + this->z * q.y - this->y * q.z;
    result.y = this->y * q.w - this->z * q.x + this->w * q.y + this->x * q.z;
    result.z = this->z * q.w + this->y * q.x - this->x * q.y + this->w * q.z;
	result.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;

	return result;
}