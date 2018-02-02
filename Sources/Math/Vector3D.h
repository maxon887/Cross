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

class Vector2D;
class Vector4D;

class Vector3D {
public:
	static const Vector3D Zero;
	static const Vector3D Up;
	static const Vector3D Forward;
	static const Vector3D Right;

	static Vector3D Cross(const Vector3D& left, const Vector3D& right);
	static float Dot(const Vector3D& left, const Vector3D& right);

	float x;
	float y;
	float z;

	Vector3D();
	Vector3D(float value);
	Vector3D(float x, float y, float z);
	Vector3D(const Vector2D& vec);
	Vector3D(const Vector2D& vec, float z);
	Vector3D(const Vector4D& vec);

	float Length() const;
	void Normaize();
	Vector3D GetNormalized() const;
	bool IsNormalized() const;
	Vector3D GetTruncated(float len) const;
	const float* GetData() const;
	void SetData(const float* data);

	bool operator == (const Vector3D& v2) const;
	bool operator != (const Vector3D& v2) const;

	Vector3D operator + (const Vector3D& v2) const;
	Vector3D operator - (const Vector3D& v2) const;
	Vector3D operator * (float value) const;
	Vector3D operator / (float value) const;

	void operator += (const Vector3D &v2);
	void operator -= (const Vector3D &v2);
	void operator *= (float value);
	void operator /= (float value);
};

}