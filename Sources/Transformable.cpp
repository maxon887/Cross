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
	rotation(0.f),
	translate(0.f),
	scale(0.f),
	model(0.f)
{ 
	rotation = Matrix::CreateIdentity();
	translate = Matrix::CreateIdentity();
	scale = Matrix::CreateIdentity();
}

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
	if(object != GetPosition()){
		Vector4D currentLook = Vector4D(0.f, 0.f, -1.f, 0.f);
		currentLook = rotation * currentLook;
		Vector3D curLook(currentLook);
		Vector3D needLook = object - GetPosition();
		curLook = curLook.Normalize();
		needLook = needLook.Normalize();
		float cosA = curLook.DotProduct(needLook);

		if(cosA == -1.f){		//opposite rotation 
			Vector3D b(Random(-1.f, 1.f), Random(-1.f, 1.f), Random(-1.f, 1.f));
			Vector3D perp = curLook.CrossProduct(b);
			SetRotate(perp, 180.f);
		}else if(cosA == 1.f){	//same look direction.Don't need do any thing
	
		}else{					//usuall random rotation
			float angle = acos(cosA);
			if(isnan(angle)){
				return;
			}
			Vector3D rotateAxis = needLook.CrossProduct(curLook);
			Quaternion currentQuat(rotation);
			Quaternion needQuat(rotateAxis, angle * 180.f / PI);
			SetRotate(currentQuat * needQuat);
		}
	}
}

Vector3D Transformable::GetPosition() const{
	return Vector3D(translate.m[0][3], translate.m[1][3], translate.m[2][3]);
}

void Transformable::SetDirection(const Vector3D& direction){
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Vector3D Transformable::GetDirection() const{
	Vector4D direction = Vector4D(0.f, 0.f, -1.f, 0.f);
	direction = rotation * direction;
	return Vector3D(direction);
}

Vector3D Transformable::GetUpVector() const{
	Vector3D direction = GetDirection();
	Vector3D right = direction.CrossProduct(Vector3D(0.f, 1.f, 0.f)) * (-1);
	right = right.Normalize();
	return right.CrossProduct(direction) * (-1);
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