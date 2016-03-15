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
#include "Vector4D.h"
#include "Vector3D.h"

using namespace cross;

Vector4D::Vector4D():
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
{ }

Vector4D::Vector4D(float x, float y, float z, float w):
	x(x),
	y(y),
	z(z),
	w(w)
{ }

Vector4D::Vector4D(const Vector3D& vec, float w):
	x(vec.x),
	y(vec.y),
	z(vec.z),
	w(w)
{ }