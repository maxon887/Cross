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
	static const Matrix Zero;
	static const Matrix Identity;

	static Matrix CreateTranslation(const Vector3D &vec);
	static Matrix CreateScale(const Vector3D &scale);
	static Matrix CreateOrthogonalProjection(float left, float right, float bottom, float top, float near, float far);
	static Matrix CreatePerspectiveProjection(float fov, float aspect, float near, float far);

	float m[4][4];

	void SetTranslation(const Vector2D &trans);
	void SetTranslation(const Vector3D &trans);
	Matrix GetTranslation() const;
	void SetScale(float scale);
	void SetScale(const Vector2D &scale);
	void SetScale(const Vector3D &scale);
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);
	Matrix GetTransposed() const;
	Matrix GetInversed() const;
	float* GetData();

	Matrix		operator + (float s) const;
	Matrix		operator - (float s) const;
	Matrix		operator * (float s) const;
	Matrix		operator / (float s) const;
	Vector4D	operator * (const Vector4D& vec) const;
	Vector3D	operator * (const Vector3D& vec) const;
	Matrix		operator * (const Matrix& m2) const;
	bool		operator == (const Matrix& m2) const;
	bool		operator != (const Matrix& m2) const;

	void operator += (float s);
	void operator -= (float s);
	void operator *= (float s);
	void operator /= (float s);

private:
	static Matrix CreateZero();
	static Matrix CreateIdentity();

	Matrix() { };
};

};