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
#include "Transformable.h"

using namespace cross;

Transformable::Transformable() :
	model(Matrix::Identity),
	translate(Matrix::Identity),
	scale(Matrix::Identity),
	rotate(Matrix::Identity),
	recalc_model(true)
{ }

void Transformable::SetPosition(const Vector2D& pos){
	translate.SetTranslation(pos);
	recalc_model = true;
}

void Transformable::SetPosition(const Vector3D& pos){
	translate.SetTranslation(pos);
	recalc_model = true;
}

void Transformable::SetScale(float factor){
	scale.SetScale(factor);
	recalc_model = true;
}

void Transformable::SetScale(const Vector2D& scaleVec){
	scale.SetScale(scaleVec);
	recalc_model = true;
}

void Transformable::SetScale(const Vector3D& scaleVec){
	scale.SetScale(scaleVec);
	recalc_model = true;
}

void Transformable::SetRotateX(float angle){
	rotate.SetRotationX(angle);
	recalc_model = true;
}

void Transformable::SetRotateY(float angle){
	rotate.SetRotationY(angle);
	recalc_model = true;
}

void Transformable::SetRotateZ(float angle){
	rotate.SetRotationZ(angle);
	recalc_model = true;
}

void Transformable::SetRotate(const Vector3D& axis, float angle){
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transformable::SetRotate(const Quaternion& quat){
	this->rotate = quat.GetMatrix();
	recalc_model = true;
}

void Transformable::SetRotate(const Matrix& rot){
	this->rotate = rot;
	recalc_model = true;
}

void Transformable::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward.Normaize();
	Vector3D right = Vector3D::Cross(Vector3D::Up, forward);
	Vector3D up =  Vector3D::Cross(forward, right);

	rotate = Matrix::Identity;

	rotate.m[0][0] = right.x;
    rotate.m[1][0] = right.y;
    rotate.m[2][0] = right.z;

    rotate.m[0][1] = up.x;
    rotate.m[1][1] = up.y;
    rotate.m[2][1] = up.z;

    rotate.m[0][2] = forward.x;
    rotate.m[1][2] = forward.y;
    rotate.m[2][2] = forward.z;
	
	recalc_model = true;
}

Vector3D Transformable::GetPosition() const{
	return Vector3D(translate.m[0][3], translate.m[1][3], translate.m[2][3]);
}

void Transformable::SetDirection(const Vector3D& direction){
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Vector3D Transformable::GetDirection() const{
	return GetForward();
}

Vector3D Transformable::GetForward() const{
	return rotate * Vector3D::Forward;
}

Vector3D Transformable::GetRight() const{
	return rotate * Vector3D::Right;
}

Vector3D Transformable::GetUp() const{
	return rotate * Vector3D::Up;
}

Matrix Transformable::GetRotation() const{
	return rotate;
}

Matrix& Transformable::GetModelMatrix(){
	if(recalc_model){
		model = translate * rotate * scale;
		recalc_model = false;
	}
	return model;
}

void Transformable::SetModelMatrix(const Matrix& mod){
	this->model = mod;
	recalc_model = false;
}