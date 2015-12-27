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
#include "Vector.h"
#include "Utils/Misc.h"

#undef near
#undef far

class Matrix{
public:
	static Matrix* CreateIdentityMatrix();
	static Matrix* CreatePerspectiveMatrix(float FoV, float aspect, float near, float far);
	~Matrix();
	float* GetData();
	void Translate(Vector vector);
	void Rotate(Vector axis, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);
private:
	Matrix();
	float* data;
};