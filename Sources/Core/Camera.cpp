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
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "System.h"

#include "Libs/TinyXML2/tinyxml2.h"

using namespace cross;
using namespace tinyxml2;

void Camera::Update(float sec){
	RecalcView();
}

bool Camera::Load(XMLElement* xml, Scene* loadingScene) {
	CROSS_RETURN(!loadingScene->GetCamera(), false, "Loading Scene already have another camera");

	view_distance = xml->DoubleAttribute("distance", 120.f);
	Matrix projection = Matrix::CreatePerspectiveProjection(45.f, system->GetAspectRatio(), 0.1f, view_distance);
	SetProjectionMatrix(projection);

	loadingScene->SetCamera(this);

	return true;
}

bool Camera::Save(XMLElement* xml, XMLDocument* doc) {
	XMLElement* cameraXML = doc->NewElement("Camera");
	cameraXML->SetAttribute("distance", view_distance);
	xml->LinkEndChild(cameraXML);
	return true;
}

Component* Camera::Clone() const {
	return new Camera(*this);
}

float Camera::GetViewDistance() const {
	return view_distance;
}

const Matrix& Camera::GetViewMatrix() const{
	return view;
}

void Camera::SetProjectionMatrix(const Matrix& projection){
	this->projection = projection;
}

const Matrix& Camera::GetProjectionMatrix() const{
	return projection;
}

void Camera::RecalcView(){
	view = Matrix::Identity;
	Vector3D direction = GetTransform()->GetDirection();
	view.m[2][0] = -direction.x;
	view.m[2][1] = -direction.y;
	view.m[2][2] = -direction.z;
	Vector3D right = GetTransform()->GetRight();
	view.m[0][0] = right.x;
	view.m[0][1] = right.y;
	view.m[0][2] = right.z;
	Vector3D up = GetTransform()->GetUp();
	view.m[1][0] = up.x;
	view.m[1][1] = up.y;
	view.m[1][2] = up.z;

	Vector3D position = GetPosition();
	Matrix posMatrix = Matrix::Identity;
	posMatrix.m[0][3] = -position.x;
	posMatrix.m[1][3] = -position.y;
	posMatrix.m[2][3] = -position.z;

	view = view * posMatrix;
}