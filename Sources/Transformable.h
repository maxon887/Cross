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
#include "Cross.h"
#include "Quaternion.h"

namespace cross{

class Transformable{
public:
	Transformable(Transformable& trans);
	Transformable();

	void SetPosition(const Vector2D& pos);
	void SetPosition(const Vector3D& pos);
	void SetScale(float factor);
	void SetScale(const Vector2D& scale);
	void SetScale(const Vector3D& scale);
	void SetRotateX(float angle);
	void SetRotateY(float angle);
	void SetRotateZ(float angle);
	void SetRotate(const Vector3D& axis, float angle);
	void SetRotate(const Quaternion& quat);
	void SetRotate(const Matrix& rotation);
	void LookAt(const Vector3D& object);
	Vector3D GetPosition() const;

	Matrix& GetModelMatrix();
protected:
	Matrix model;
	Matrix scale;
	Matrix translate;
	Matrix rotation;
	bool recalc_model;
};

}