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
#include "Transform.h"
#include "Entity.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

Transform::Transform(const Vector3D& position) {
	SetPosition(position);
}

Component* Transform::Clone() const {
	return new Transform(*this);
}

bool Transform::Load(tinyxml2::XMLElement* xml, Scene*) {
	XMLElement* posXML = xml->FirstChildElement("Position");
	if(posXML) {
		double x = posXML->DoubleAttribute("x");
		double y = posXML->DoubleAttribute("y");
		double z = posXML->DoubleAttribute("z");
		SetPosition(Vector3D((float)x, (float)y, (float)z));
	}
	XMLElement* rotXML = xml->FirstChildElement("Rotation");
	if(rotXML) {
		double x = rotXML->DoubleAttribute("x");
		double y = rotXML->DoubleAttribute("y");
		double z = rotXML->DoubleAttribute("z");
		double angle = rotXML->DoubleAttribute("angle");
		SetRotate(Quaternion(Vector3D((float)x, (float)y, (float)z), (float)angle));
	}
	XMLElement* scaleXML = xml->FirstChildElement("Scale");
	if(scaleXML) {
		double x = scaleXML->DoubleAttribute("x");
		double y = scaleXML->DoubleAttribute("y");
		double z = scaleXML->DoubleAttribute("z");
		SetScale(Vector3D((float)x, (float)y, (float)z));
	}
	return true;
}

bool Transform::Save(XMLElement* xml, XMLDocument* doc) {
	XMLElement* transformXML = doc->NewElement("Transform");

	XMLElement* posXML = doc->NewElement("Position");
	Vector3D pos = GetEntity()->GetTransform()->GetPosition();
	posXML->SetAttribute("x", pos.x);
	posXML->SetAttribute("y", pos.y);
	posXML->SetAttribute("z", pos.z);
	transformXML->LinkEndChild(posXML);

	XMLElement* rotXML = doc->NewElement("Rotation");
	Quaternion rot = GetEntity()->GetTransform()->GetRotate().GetNormalized();
	Vector3D axis = rot.GetAxis();
	float angle = rot.GetAngle();
	rotXML->SetAttribute("x", axis.x);
	rotXML->SetAttribute("y", axis.y);
	rotXML->SetAttribute("z", axis.z);
	rotXML->SetAttribute("angle", angle);
	transformXML->LinkEndChild(rotXML);

	XMLElement* scaleXML = doc->NewElement("Scale");
	Vector3D scale = GetEntity()->GetTransform()->GetScale();
	scaleXML->SetAttribute("x", scale.x);
	scaleXML->SetAttribute("y", scale.y);
	scaleXML->SetAttribute("z", scale.z);
	transformXML->LinkEndChild(scaleXML);

	xml->LinkEndChild(transformXML);
	return true;
}

void Transform::SetPosition(const Vector2D& pos) {
	translate.SetTranslation(pos);
	recalc_model = true;
}

void Transform::SetPosition(const Vector3D& pos) {
	translate.SetTranslation(pos);
	recalc_model = true;
}

void Transform::SetPosition(const Matrix& pos) {
	this->translate = pos;
	recalc_model = true;
}

Vector3D Transform::GetPosition() const {
	return Vector3D(translate.m[0][3], translate.m[1][3], translate.m[2][3]);
}

void Transform::SetScale(float factor) {
	scale.SetScale(factor);
	recalc_model = true;
}

void Transform::SetScale(const Vector2D& scaleVec) {
	scale.SetScale(scaleVec);
	recalc_model = true;
}

void Transform::SetScale(const Vector3D& scaleVec) {
	scale.SetScale(scaleVec);
	recalc_model = true;
}

void Transform::SetScale(const Matrix& scale) {
	this->scale = scale;
	recalc_model = true;
}

Vector3D Transform::GetScale() const{
	return Vector3D(scale.m[0][0], scale.m[1][1], scale.m[2][2]);
}

void Transform::SetRotateX(float angle) {
	rotate.SetRotationX(angle);
	recalc_model = true;
}

void Transform::SetRotateY(float angle) {
	rotate.SetRotationY(angle);
	recalc_model = true;
}

void Transform::SetRotateZ(float angle) {
	rotate.SetRotationZ(angle);
	recalc_model = true;
}

void Transform::SetRotate(const Vector3D& axis, float angle) {
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transform::SetRotate(const Quaternion& quat) {
	this->rotate = quat.GetMatrix();
	recalc_model = true;
}

void Transform::SetRotate(const Matrix& rot) {
	this->rotate = rot;
	recalc_model = true;
}

Quaternion Transform::GetRotate() const {
	return Quaternion(rotate);
}

void Transform::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward.Normaize();
	Vector3D right = Vector3D::Cross(Vector3D::Up, forward).GetNormalized();
	Vector3D up =  Vector3D::Cross(forward, right).GetNormalized();

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

void Transform::SetDirection(const Vector3D& direction) {
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Vector3D Transform::GetDirection() const {
	return GetForward();
}

Vector3D Transform::GetForward() const {
	return rotate * Vector3D::Forward;
}

Vector3D Transform::GetRight() const {
	return rotate * Vector3D::Right;
}

Vector3D Transform::GetUp() const {
	return rotate * Vector3D::Up;
}

Matrix Transform::GetRotation() const {
	return rotate;
}

Matrix& Transform::GetModelMatrix() {
	if(recalc_model){
		model = translate * rotate * scale;
		recalc_model = false;
	}
	return model;
}

void Transform::SetModelMatrix(const Matrix& mod){
	this->model = mod;
	recalc_model = false;
}