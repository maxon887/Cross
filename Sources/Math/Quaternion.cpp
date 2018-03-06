/*	Copyright � 2015 Maksim Lukyanov

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

const Quaternion Quaternion::Identity = Quaternion();

float Quaternion::DotProduct(const Quaternion& left, const Quaternion& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}

Quaternion::Quaternion():
	x(0.f),
	y(0.f),
	z(0.f),
	w(1.f)
{ }

Quaternion::Quaternion(const Vector3D& inAxis, float angle) { 
	Vector3D axis = inAxis.GetNormalized();
	float halfAngle = angle * 0.5f;
	float cosA = cos(halfAngle / 180.f * PI);
	float sinA = sin(halfAngle / 180.f * PI);
	this->x = axis.x * sinA;
	this->y = axis.y * sinA;
	this->z = axis.z * sinA;
	this->w = cosA;
}

Quaternion::Quaternion(const Matrix& m) {
	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
	if(trace > 0) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		this->w = 0.25f / s;
		this->x = (m.m[1][2] - m.m[2][1]) * s;
		this->y = (m.m[2][0] - m.m[0][2]) * s;
		this->z = (m.m[0][1] - m.m[1][0]) * s;
	} else {
		if(m.m[0][0] > m.m[1][1] && m.m[0][0] > m.m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]);
			this->w = (m.m[1][2] - m.m[2][1]) / s;
			this->x = 0.25f * s;
			this->y = (m.m[1][0] + m.m[0][1]) / s;
			this->z = (m.m[2][0] + m.m[0][2]) / s;
		} else if(m.m[1][1] > m.m[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]);
			this->w = (m.m[2][0] - m.m[0][2]) / s;
			this->x = (m.m[1][0] + m.m[0][1]) / s;
			this->y = 0.25f * s;
			this->z = (m.m[2][1] + m.m[1][2]) / s;
		} else {
			float s = 2.0f * sqrtf(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]);
			this->w = (m.m[0][1] - m.m[1][0]) / s;
			this->x = (m.m[2][0] + m.m[0][2]) / s;
			this->y = (m.m[2][1] + m.m[1][2]) / s;
			this->z = 0.25f * s;
		}
	}
	/*
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2]; // trace of m.martix
	if (tr > 0.0f) {	 // if trace positive than "w" is biggest com.mponent
		this->x = m.m[1][2] - m.m[2][1];
		this->y = m.m[2][0] - m.m[0][2];
		this->z = m.m[0][1] - m.m[1][0];
		this->w = tr + 1.0f;
	} else				 // Som.me of vector com.mponents is bigger
	if( (m.m[0][0] > m.m[1][1] ) && ( m.m[0][0] > m.m[2][2]) ) {
		this->x = 1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2];
		this->y = m.m[1][0] + m.m[0][1];
		this->z = m.m[2][0] + m.m[0][2];
		this->w = m.m[1][2] - m.m[2][1];
	} else 
	if ( m.m[1][1] > m.m[2][2] ) {
		this->x = m.m[1][0] + m.m[0][1];
		this->y = 1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2];
		this->z = m.m[2][1] + m.m[1][2];
		this->w = m.m[2][0] - m.m[0][2];
	} else {
		this->x = m.m[2][0] + m.m[0][2];
		this->y = m.m[2][1] + m.m[1][2];
		this->z = 1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1];
		this->w = m.m[0][1] - m.m[1][0];
	}*/
}

void Quaternion::Scale(float scale) {
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
	this->w *= scale;
}

float Quaternion::Norm() const {
	return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}

float Quaternion::Length() const {
	return sqrt(this->Norm());
}

Quaternion Quaternion::GetConjugated() const {
	Quaternion result = *this;
	result.x *= -1;
	result.y *= -1;
	result.z *= -1;
	return result;
}

Quaternion Quaternion::GetInversed() const {
	return GetConjugated() / Norm();
}

Quaternion Quaternion::GetNormalized() const {
	float len = Length();
	Quaternion result;
	result.x = x / len;
	result.y = y / len;
	result.z = z / len;
	result.w = w / len;
	return result;
}

Matrix Quaternion::GetMatrix() const {
	Matrix m = Matrix::Identity;
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	float s  = 2.0f / this->Norm();  // 4 mul 3 add 1 div
	x2 = this->x * s;	y2 = this->y * s;	z2 = this->z * s;
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

Vector3D Quaternion::GetAxis() const {
	Vector3D axis;
	float sinA = sqrt(1.f - w * w);
	if(sinA > 0){
		axis.x = x / sinA;
		axis.y = y / sinA;
		axis.z = z / sinA;
	}
	return axis;
}

float Quaternion::GetAngle() const {
	return 2 * acos(w) * 180.f / PI;
}

Quaternion Quaternion::operator+(const Quaternion& q) const {
	Quaternion result;
	result.x = this->x + q.x;
	result.y = this->y + q.y;
	result.z = this->z + q.z;
	result.w = this->w + q.w;
	return result;
}

Quaternion Quaternion::operator-(const Quaternion& q) const {
	Quaternion result;
	result.x = this->x - q.x;
	result.y = this->y - q.y;
	result.z = this->z - q.z;
	result.w = this->w - q.w;
	return result;
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
	Quaternion result;
	result.x = this->x * q.w + this->w * q.x + this->z * q.y - this->y * q.z;
	result.y = this->y * q.w - this->z * q.x + this->w * q.y + this->x * q.z;
	result.z = this->z * q.w + this->y * q.x - this->x * q.y + this->w * q.z;
	result.w = this->w * q.w - this->x * q.x - this->y * q.y - this->z * q.z;
	return result;
}

Vector3D Quaternion::operator*(const Vector3D& vec) const {
	Quaternion vecQ;
	vecQ.x = vec.x;
	vecQ.y = vec.y;
	vecQ.z = vec.z;
	vecQ.w = 0;
	Quaternion quat = (*this) * vecQ * GetInversed();
	return Vector3D(quat.x, quat.y, quat.z);
}

Quaternion Quaternion::operator * (float v) const {
	Quaternion result;
	result.x = this->x * v;
	result.y = this->y * v;
	result.z = this->z * v;
	result.w = this->w * v;
	return result;
}

Quaternion Quaternion::operator / (float v) const {
	Quaternion result;
	result.x = this->x / v;
	result.y = this->y / v;
	result.z = this->z / v;
	result.w = this->w / v;
	return result;
}