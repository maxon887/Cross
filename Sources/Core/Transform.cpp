/*	Copyright © 2018 Maksim Lukyanov

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
	posXML->SetAttribute("x", position.x);
	posXML->SetAttribute("y", position.y);
	posXML->SetAttribute("z", position.z);
	transformXML->LinkEndChild(posXML);

	XMLElement* rotXML = doc->NewElement("Rotation");
	Vector3D axis = rotation.GetAxis();
	float angle = rotation.GetAngle();
	rotXML->SetAttribute("x", axis.x);
	rotXML->SetAttribute("y", axis.y);
	rotXML->SetAttribute("z", axis.z);
	rotXML->SetAttribute("angle", angle);
	transformXML->LinkEndChild(rotXML);

	XMLElement* scaleXML = doc->NewElement("Scale");
	scaleXML->SetAttribute("x", scale.x);
	scaleXML->SetAttribute("y", scale.y);
	scaleXML->SetAttribute("z", scale.z);
	transformXML->LinkEndChild(scaleXML);

	xml->LinkEndChild(transformXML);
	return true;
}

Vector3D Transform::GetPosition() const {
	return position;
}

void Transform::SetPosition(const Vector2D& pos) {
	position = pos;
	recalc_model = true;
}

void Transform::SetPosition(const Vector3D& pos) {
	position = pos;
	recalc_model = true;
}

void Transform::SetPosition(const Matrix& pos) {
	position.x = pos.m[0][3];
	position.y = pos.m[1][3];
	position.z = pos.m[2][3];
	recalc_model = true;
}

Vector3D Transform::GetScale() const {
	return scale;
}

void Transform::SetScale(float factor) {
	scale = Vector3D(factor);
	recalc_model = true;
}

void Transform::SetScale(const Vector2D& scaleVec) {
	scale = scaleVec;
	recalc_model = true;
}

void Transform::SetScale(const Vector3D& scaleVec) {
	scale = scaleVec;
	recalc_model = true;
}

void Transform::SetScale(const Matrix& scaleMat) {
	scale.x = scaleMat.m[0][0];
	scale.y = scaleMat.m[1][1];
	scale.z = scaleMat.m[2][2];
	recalc_model = true;
}

Quaternion Transform::GetRotate() const {
	return rotation;
}

void Transform::SetRotate(const Vector3D& axis, float angle) {
	Quaternion quat(axis, angle);
	SetRotate(quat);
}

void Transform::SetRotate(const Quaternion& quat) {
	rotation = quat.GetMatrix();
	recalc_model = true;
}

void Transform::SetRotate(const Matrix& rot) {
	rotation = rot;
	recalc_model = true;
}

void Transform::LookAt(const Vector3D& object){
	Vector3D forward = object - GetPosition();
	forward.Normaize();
	Vector3D right = Vector3D::Cross(Vector3D::Up, forward).GetNormalized();
	Vector3D up =  Vector3D::Cross(forward, right).GetNormalized();

	Matrix rot = Matrix::Identity;

	rot.m[0][0] = right.x;
	rot.m[1][0] = right.y;
	rot.m[2][0] = right.z;

	rot.m[0][1] = up.x;
	rot.m[1][1] = up.y;
	rot.m[2][1] = up.z;

	rot.m[0][2] = forward.x;
	rot.m[1][2] = forward.y;
	rot.m[2][2] = forward.z;

	rotation = rot;
	
	recalc_model = true;
}

Vector3D Transform::GetDirection() const {
	return GetForward();
}

Vector3D Transform::GetForward() const {
	return rotation * Vector3D::Forward;
}

Vector3D Transform::GetRight() const {
	return rotation * Vector3D::Right;
}

Vector3D Transform::GetUp() const {
	return rotation * Vector3D::Up;
}

void Transform::SetDirection(const Vector3D& direction) {
	Vector3D lookAt = this->GetPosition() + direction;
	LookAt(lookAt);
}

Matrix& Transform::GetModelMatrix() {
	if(recalc_model) {
		Matrix translate = Matrix::Identity;
		translate.SetTranslation(position);
		Matrix scaleMat = Matrix::Identity;
		scaleMat.SetScale(scale);
		model = translate * rotation.GetMatrix() * scaleMat;
		recalc_model = false;
	}
	return model;
}