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
#include <math.h>

Matrix::Matrix(){
	data = new float[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix::~Matrix(){
	delete[] data;
}

float* Matrix::GetData(){
	return data;
}

void Matrix::Translate(Vector2D vec){
	data[3] = vec.x;
	data[7] = vec.y;
}

void Matrix::RotateZ(float degrees){
	float cosA = cos(degrees*PI / 180.f);
	float sinA = sin(degrees*PI / 180.f);
	data[0] = cosA;
	data[1] = -sinA;
	data[4] = sinA;
	data[5] = cosA;
}