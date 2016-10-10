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

Transformable::Transformable() :
	recalc_model(true),
	rotation(Matrix::Identity),
	position(0.f),
	scale(1.f),
	model(Matrix::Identity)
{ }

void Transformable::SetPosition(const Vector2D& pos){
	SetPosition(Vector3D(pos));
}

void Transformable::SetPosition(const Vector3D& pos){
	position = pos;
	recalc_model = true;
}

void Transformable::SetScale(float factor){
	SetScale(Vector3D(factor));
}

void Transformable::SetScale(const Vector2D& scaleVec){
	SetScale(Vector3D(scaleVec));
}

void Transformable::SetScale(const Vector3D& scaleVec){
	scale = scaleVec;
	recalc_model = true;
}

void Transformable::SetRotateX(float angle){
	rotation = Quaternion(Vector3D::Right, angle);
	recalc_model = true;
}

void Transformable::SetRotateY(float angle){
	rotation = Quaternion(Vector3D::Up, angle);
	recalc_model = true;
}

void Transformable::SetRotateZ(float angle){
	rotation = Quaternion(Vector3D::Forward, angle);
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

	Matrix rotateMat = Matrix::Identity;

	rotateMat.m[0][0] = right.x;
    rotateMat.m[1][0] = right.y;
    rotateMat.m[2][0] = right.z;

    rotateMat.m[0][1] = up.x;
    rotateMat.m[1][1] = up.y;
    rotateMat.m[2][1] = up.z;

    rotateMat.m[0][2] = forward.x;
    rotateMat.m[1][2] = forward.y;
    rotateMat.m[2][2] = forward.z;
	
	recalc_model = true;
}

Vector3D Transformable::GetPosition() const{
	return position;
}

void Transformable::SetDirection(const Vector3D& direction){
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Vector3D Transformable::GetDirection() const{
	return GetForward();
}

Vector3D Transformable::GetForward() const{
	//Vector4D forward = Vector4D(0.f, 0.f, 1.f, 0.f);
	//forward = rotation.GetMatrix() * forward;
	//return Vector3D(forward);
	return rotation * Vector3D::Forward;
}

Vector3D Transformable::GetRight() const{
	//Vector4D right = Vector4D(1.f, 0.f, 0.f, 0.f);
	//right = rotation.GetMatrix() * right;
	//return Vector3D(right);
	return rotation * Vector3D::Right;
}

Vector3D Transformable::GetUp() const{
	//Vector4D up = Vector4D(0.f, 1.f, 0.f, 0.f);
	//up = rotation.GetMatrix() * up;
	//return Vector3D(up);
	return rotation * Vector3D::Up;
}

Quaternion Transformable::GetRotation() const{
	return rotation;
}

Matrix Transformable::GetNormalMatrix() const{
	Matrix scaleMat = Matrix::CreateScale(scale);
	Matrix rotateMat = rotation.GetMatrix();
	return rotateMat * scaleMat;
}

Matrix& Transformable::GetModelMatrix(){
	if(recalc_model){
		Matrix translateMat = Matrix::CreateTranslation(position);
		Matrix scaleMat = Matrix::CreateScale(scale);
		Matrix rotateMat = rotation.GetMatrix();
		model = translateMat * rotateMat * scaleMat;
		recalc_model = false;
	}
	return model;
}

void Transformable::SetModelMatrix(const Matrix& mod){
	this->model = mod;
	recalc_model = false;
}