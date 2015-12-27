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

Matrix* Matrix::CreateIdentityMatrix(){
	Matrix* mat = new Matrix();
	mat->data = new float[16]{
		1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};
	return mat;
}

Matrix* Matrix::CreatePerspectiveMatrix(float FoV, float aspect, float near, float far){
	Matrix* mat = new Matrix();
	mat->data = new float[16];
	for(int i = 0; i < 16; i++){
		mat->data[i] = 0;
	}

	float angle = (FoV / 180.0f) * PI;
	float f = 1.0f / tan(angle * 0.5f);

	mat->data[0*4 + 0] = f / aspect;
	mat->data[1 * 4 + 1] = f;
	mat->data[2 * 4 + 2] = (far + near) / (near - far);
	mat->data[2 * 4 + 3] = -1.0f;
	mat->data[3 * 4 + 2] = (2.0f * far*near) / (near - far);

	return mat;
}

Matrix::Matrix(){ }

Matrix::~Matrix(){
	delete[] data;
}

float* Matrix::GetData(){
	return data;
}

void Matrix::Translate(Vector vec){
	data[3] = vec.x;
	data[7] = vec.y;
	data[11] = vec.z;
}

void Matrix::Rotate(Vector axis, float degree){
	Vector tA(1.0f, 0.0f, 0.0f);
	Vector tB(0.0f, 1.0f, 0.0f);
	Vector tC = tA.CrossProduct(tB);


	Vector B(0.0f, 0.0f, 1.0f);
	Vector C = axis.CrossProduct(B);
	B = C.CrossProduct(axis);
	delete[] data;
	data = new float[16]{
			B.x, C.x, axis.x, 0.0f,
			B.y, C.y, axis.y, 0.0f,
			B.z, C.z, axis.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
	};
}

void Matrix::RotateX(float degrees){
	float cosA = cos(degrees*PI / 180.f);
	float sinA = sin(degrees*PI / 180.f);
	data[5] = cosA;
	data[6] = -sinA;
	data[9] = sinA;
	data[10] = cosA;
}

void Matrix::RotateY(float degrees){
	float cosA = cos(degrees*PI / 180.f);
	float sinA = sin(degrees*PI / 180.f);
	data[0] = cosA;
	data[2] = sinA;
	data[8] = -sinA;
	data[10] = cosA;
}

void Matrix::RotateZ(float degrees){
	float cosA = cos(degrees*PI / 180.f);
	float sinA = sin(degrees*PI / 180.f);
	data[0] = cosA;
	data[1] = -sinA;
	data[4] = sinA;
	data[5] = cosA;
}