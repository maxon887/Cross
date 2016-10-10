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
#include "Matrix.h"
#include "Cross.h"

#include <math.h>

using namespace cross;

const Matrix Matrix::Zero = Matrix::CreateZero();
const Matrix Matrix::Identity = Matrix::CreateIdentity();

Matrix Matrix::CreateTranslation(const Vector3D &vec){
	Matrix m = Matrix::Identity;
	m.SetTranslation(vec);
	return m;
}

Matrix Matrix::CreateScale(const Vector3D &scale){
	Matrix m = Matrix::Identity;
	m.SetScale(scale);
	return m;
}

Matrix Matrix::CreateOrthogonalProjection(float left, float right, float bottom, float top, float near, float far){
	Matrix m;

	m.m[0][0] = 2.0f / (right - left);
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = (-1.0f) * (right + left) / (right - left);

	m.m[1][0] = 0.0f;
	m.m[1][1] = 2.0f / (top - bottom);
	m.m[1][2] = 0.0f;
	m.m[1][3] = (-1.0f) * (top + bottom) / (top - bottom);

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = -2.0f / (far - near);
	m.m[2][3] = (-1.0f) * (far + near) / (far - near);

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;
	
	return m;
}

Matrix Matrix::CreatePerspectiveProjection(float fov, float aspect, float near, float far){
	Matrix m;
	float tanFov = tan(fov / 2);

	m.m[0][0] = 1.f / (aspect * tanFov);
	m.m[0][1] = 0.0f;
	m.m[0][2] = 0.0f;
	m.m[0][3] = 0.0f;

	m.m[1][0] = 0.0f;
	m.m[1][1] = 1.f / tanFov;
	m.m[1][2] = 0.0f;
	m.m[1][3] = 0.0f;

	m.m[2][0] = 0.0f;
	m.m[2][1] = 0.0f;
	m.m[2][2] = (-1) * (far + near) / (far - near);
	m.m[2][3] = (-1) * (2 * far * near) / (far - near);

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = -1.f;
	m.m[3][3] = 0.0f;

	return m;
}

float* Matrix::GetData(){
	return (float*)m;
}

void Matrix::SetTranslation(const Vector2D &trans){
	m[0][3] = trans.x;
	m[1][3] = trans.y;
}

void Matrix::SetTranslation(const Vector3D &trans){
	m[0][3] = trans.x;
	m[1][3] = trans.y;
	m[2][3] = trans.z;
}

void Matrix::SetScale(float scale){
	m[0][0] = scale;
	m[1][1] = scale;
	m[2][2] = scale;
}

void Matrix::SetScale(const Vector2D &scale){
	m[0][0] = scale.x;
	m[1][1] = scale.y;
}

void Matrix::SetScale(const Vector3D &scale){
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Matrix::SetRotationX(float angle){
	float cosA = cos(angle / 180.f * PI);
	float sinA = sin(angle / 180.f * PI);
	m[1][1] = cosA;
	m[1][2] = -sinA;
	m[2][1] = sinA;
	m[2][2] = cosA;
}

void Matrix::SetRotationY(float angle){
	float cosA = cos(angle / 180.f * PI);
	float sinA = sin(angle / 180.f * PI);
	m[0][0] = cosA;
	m[0][2] = sinA;
	m[2][0] = -sinA;
	m[2][2] = cosA;
}

void Matrix::SetRotationZ(float angle){
	float cosA = cos(angle / 180.f * PI);
	float sinA = sin(angle / 180.f * PI);
	m[0][0] = cosA;
	m[0][1] = -sinA;
	m[1][0] = sinA;
	m[1][1] = cosA;
}

Matrix Matrix::Transpose() const{
	Matrix res;
	res.m[0][0] = m[0][0]; res.m[0][1] = m[1][0]; res.m[0][2] = m[2][0]; res.m[0][3] = m[3][0];
	res.m[1][0] = m[0][1]; res.m[1][1] = m[1][1]; res.m[1][2] = m[2][1]; res.m[1][3] = m[3][1];
	res.m[2][0] = m[0][2]; res.m[2][1] = m[1][2]; res.m[2][2] = m[2][2]; res.m[2][3] = m[3][2];
	res.m[3][0] = m[0][3]; res.m[3][1] = m[1][3]; res.m[3][2] = m[2][3]; res.m[3][3] = m[3][3];
	return res;
}

Matrix Matrix::Inverse() const{
	Matrix inverse = *this;

	float inv[16], det;
	float* m = *(inverse.m);

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if(det == 0)
		throw CrossException("Determinant equal 0");

	det = 1.0f / det;

	for(int i = 0; i < 16; i++)
		m[i] = inv[i] * det;
	return inverse;
}

Matrix Matrix::operator + (float s) const{
	Matrix res(*this);
	res += s;
	return res;
}

void Matrix::operator += (float s){
	m[0][0] += s; m[0][1] += s; m[0][2] += s; m[0][3] += s;
	m[1][0] += s; m[1][1] += s; m[1][2] += s; m[1][3] += s;
	m[2][0] += s; m[2][1] += s; m[2][2] += s; m[2][3] += s;
	m[3][0] += s; m[3][1] += s; m[3][2] += s; m[3][3] += s;
}

Matrix Matrix::operator - (float s) const{
	Matrix res(*this);
	res -= s;
	return res;
}

void Matrix::operator -= (float s){
	m[0][0] -= s; m[0][1] -= s; m[0][2] -= s; m[0][3] -= s;
	m[1][0] -= s; m[1][1] -= s; m[1][2] -= s; m[1][3] -= s;
	m[2][0] -= s; m[2][1] -= s; m[2][2] -= s; m[2][3] -= s;
	m[3][0] -= s; m[3][1] -= s; m[3][2] -= s; m[3][3] -= s;
}

Matrix Matrix::operator * (float s) const{
	Matrix res(*this);
	res *= s;
	return res;
}

void Matrix::operator *= (float s){
	m[0][0] *= s; m[0][1] *= s; m[0][2] *= s; m[0][3] *= s;
	m[1][0] *= s; m[1][1] *= s; m[1][2] *= s; m[1][3] *= s;
	m[2][0] *= s; m[2][1] *= s; m[2][2] *= s; m[2][3] *= s;
	m[3][0] *= s; m[3][1] *= s; m[3][2] *= s; m[3][3] *= s;
}

Matrix Matrix::operator / (float s) const{
	Matrix res(*this);
	res /= s;
	return res;
}

void Matrix::operator /= (float s){
	m[0][0] /= s; m[0][1] /= s; m[0][2] /= s; m[0][3] /= s;
	m[1][0] /= s; m[1][1] /= s; m[1][2] /= s; m[1][3] /= s;
	m[2][0] /= s; m[2][1] /= s; m[2][2] /= s; m[2][3] /= s;
	m[3][0] /= s; m[3][1] /= s; m[3][2] /= s; m[3][3] /= s;
}

Vector4D Matrix::operator * (const Vector4D &vec) const{
	Vector4D res;
	res.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3] * vec.w;
	res.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3] * vec.w;
	res.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3] * vec.w;
	res.w = m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3] * vec.w;
	return res;
}

Matrix Matrix::operator * (const Matrix& mat) const{
	Matrix res;
	res.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0] + m[0][3] * mat.m[3][0];
	res.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1] + m[0][3] * mat.m[3][1];
	res.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] + m[0][2] * mat.m[2][2] + m[0][3] * mat.m[3][2];
	res.m[0][3] = m[0][0] * mat.m[0][3] + m[0][1] * mat.m[1][3] + m[0][2] * mat.m[2][3] + m[0][3] * mat.m[3][3];

	res.m[1][0] = m[1][0] * mat.m[0][0] + m[1][1] * mat.m[1][0] + m[1][2] * mat.m[2][0] + m[1][3] * mat.m[3][0];
	res.m[1][1] = m[1][0] * mat.m[0][1] + m[1][1] * mat.m[1][1] + m[1][2] * mat.m[2][1] + m[1][3] * mat.m[3][1];
	res.m[1][2] = m[1][0] * mat.m[0][2] + m[1][1] * mat.m[1][2] + m[1][2] * mat.m[2][2] + m[1][3] * mat.m[3][2];
	res.m[1][3] = m[1][0] * mat.m[0][3] + m[1][1] * mat.m[1][3] + m[1][2] * mat.m[2][3] + m[1][3] * mat.m[3][3];

	res.m[2][0] = m[2][0] * mat.m[0][0] + m[2][1] * mat.m[1][0] + m[2][2] * mat.m[2][0] + m[2][3] * mat.m[3][0];
	res.m[2][1] = m[2][0] * mat.m[0][1] + m[2][1] * mat.m[1][1] + m[2][2] * mat.m[2][1] + m[2][3] * mat.m[3][1];
	res.m[2][2] = m[2][0] * mat.m[0][2] + m[2][1] * mat.m[1][2] + m[2][2] * mat.m[2][2] + m[2][3] * mat.m[3][2];
	res.m[2][3] = m[2][0] * mat.m[0][3] + m[2][1] * mat.m[1][3] + m[2][2] * mat.m[2][3] + m[2][3] * mat.m[3][3];

	res.m[3][0] = m[3][0] * mat.m[0][0] + m[3][1] * mat.m[1][0] + m[3][2] * mat.m[2][0] + m[3][3] * mat.m[3][0];
	res.m[3][1] = m[3][0] * mat.m[0][1] + m[3][1] * mat.m[1][1] + m[3][2] * mat.m[2][1] + m[3][3] * mat.m[3][1];
	res.m[3][2] = m[3][0] * mat.m[0][2] + m[3][1] * mat.m[1][2] + m[3][2] * mat.m[2][2] + m[3][3] * mat.m[3][2];
	res.m[3][3] = m[3][0] * mat.m[0][3] + m[3][1] * mat.m[1][3] + m[3][2] * mat.m[2][3] + m[3][3] * mat.m[3][3];
	return res;
}

Matrix Matrix::CreateZero(){
	Matrix m;
	m.m[0][0] = 0.0f; m.m[0][1] = 0.0f; m.m[0][2] = 0.0f; m.m[0][3] = 0.0f;
	m.m[1][0] = 0.0f; m.m[1][1] = 0.0f; m.m[1][2] = 0.0f; m.m[1][3] = 0.0f;
	m.m[2][0] = 0.0f; m.m[2][1] = 0.0f; m.m[2][2] = 0.0f; m.m[2][3] = 0.0f;
	m.m[3][0] = 0.0f; m.m[3][1] = 0.0f; m.m[3][2] = 0.0f; m.m[3][3] = 0.0f;
	return m;
}

Matrix Matrix::CreateIdentity(){
	Matrix m;
	m.m[0][0] = 1.0f; m.m[0][1] = 0.0f; m.m[0][2] = 0.0f; m.m[0][3] = 0.0f;
	m.m[1][0] = 0.0f; m.m[1][1] = 1.0f; m.m[1][2] = 0.0f; m.m[1][3] = 0.0f;
	m.m[2][0] = 0.0f; m.m[2][1] = 0.0f; m.m[2][2] = 1.0f; m.m[2][3] = 0.0f;
	m.m[3][0] = 0.0f; m.m[3][1] = 0.0f; m.m[3][2] = 0.0f; m.m[3][3] = 1.0f;
	return m;
}