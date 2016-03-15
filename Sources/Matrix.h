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

class Vector2D;
class Vector3D;
class Vector4D;

class Matrix{
public:
	static Matrix CreateZero();
	static Matrix CreateIdentity();
	static Matrix CreateTranslation(const Vector3D &vec);
	static Matrix CreateOrthogonalProjection(float left, float right, float bottom, float top, float near, float far);
	static Matrix CreatePerspectiveProjection(float fov, float aspect, float near, float far);

	float m[4][4];

	void SetTranslation(const Vector2D &trans);
	void SetTranslation(const Vector3D &trans);
	void SetScale(float scale);
	void SetScale(const Vector2D &scale);
	void SetScale(const Vector3D &scale);
	void SetRotationZ(float angle);
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	Matrix Transpose();
	float* GetData();

	Matrix operator + (float s) const;
	void operator += (float s);
	Matrix operator - (float s) const;
	void operator -= (float s);
	Matrix operator * (float s) const;
	void operator *= (float s);
	Matrix operator / (float s) const;
	void operator /= (float s);
	Vector4D operator * (const Vector4D& vec) const;
	Matrix operator * (const Matrix &m2) const;
};

};