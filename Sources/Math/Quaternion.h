/*	Copyright © 2018 Maksim Lukyanov

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

class Vector3D;
class Matrix;

class Quaternion {
public:
	static const Quaternion Identity;

	static float DotProduct(const Quaternion& left, const Quaternion& right);
	/* Creates identity quaternion */
	Quaternion();
	/* Constructor which takes rotation axis and angle in degrees */
	Quaternion(const Vector3D& axis, float angle);
	/* Constructor which create quaternion by rotation matrix */
	Quaternion(const Matrix& rotation);

	void Scale(float scale);
	float Norm() const;  
	float Length() const;
	Quaternion GetConjugated() const;
	Quaternion GetInversed() const;
	Quaternion GetNormalized() const;
	Matrix GetMatrix() const;
	Vector3D GetAxis() const;
	float GetAngle() const;

	Quaternion operator + (const Quaternion& q) const;
	Quaternion operator - (const Quaternion& q) const;
	Quaternion operator * (const Quaternion& q) const;
	Vector3D   operator * (const Vector3D& vec) const;
	Quaternion operator * (float value) const;
	Quaternion operator / (float value) const;

	float x;
	float y;
	float z;
	float w;
};

}