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
#include "Launcher.h"

#include <math.h>

using namespace cross;

Transformable::Transformable(Transformable& trans) :
	model(trans.model),
	scale(trans.scale),
	translate(trans.translate),
	rotation(trans.rotation),
	recalc_model(trans.recalc_model)
{ }

Transformable::Transformable() :
	recalc_model(true),
	rotation(Matrix::Identity),
	translate(Matrix::Identity),
	scale(Matrix::Identity),
	model(0.f)
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
	rotation.SetRotationX(angle);
	recalc_model = true;
}

void Transformable::SetRotateY(float angle){
	rotation.SetRotationY(angle);
	recalc_model = true;
}

void Transformable::SetRotateZ(float angle){
	rotation.SetRotationZ(angle);
	recalc_model = true;
}

void Transformable::SetRotate(const Vector3D& axis, float angle){
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transformable::SetRotate(const Quaternion& quat){
	this->rotation = quat.GetMatrix();
	recalc_model = true;
}

void Transformable::SetRotate(const Matrix& rot){
	this->rotation = rot;
	recalc_model = true;
}

void Transformable::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward = forward.Normalize();
	Vector3D right = Vector3D::Up.CrossProduct(forward);
	Vector3D up = forward.CrossProduct(right);

	rotation = Matrix::Identity;

	rotation.m[0][0] = right.x;
    rotation.m[1][0] = right.y;
    rotation.m[2][0] = right.z;

    rotation.m[0][1] = up.x;
    rotation.m[1][1] = up.y;
    rotation.m[2][1] = up.z;

    rotation.m[0][2] = forward.x;
    rotation.m[1][2] = forward.y;
    rotation.m[2][2] = forward.z;
	
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
	Vector4D forward = Vector4D(0.f, 0.f, 1.f, 0.f);
	forward = rotation * forward;
	return Vector3D(forward);
}

Vector3D Transformable::GetRight() const{
	Vector4D right = Vector4D(1.f, 0.f, 0.f, 0.f);
	right = rotation * right;
	return Vector3D(right);
}

Vector3D Transformable::GetUp() const{
	Vector4D up = Vector4D(0.f, 1.f, 0.f, 0.f);
	up = rotation * up;
	return Vector3D(up);
}

Matrix Transformable::GetRotation() const{
	return rotation;
}

Matrix& Transformable::GetModelMatrix(){
	if(recalc_model){
		model = translate * rotation * scale;
		recalc_model = false;
	}
	return model;
}

void Transformable::SetModelMatrix(const Matrix& mod){
	this->model = mod;
	recalc_model = false;
}

Matrix Transformable::GetNormalMatrix(){
	return rotation * scale;
}