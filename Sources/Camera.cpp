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
#include "Launcher.h"

using namespace cross;

Camera::Camera(){
	view = Matrix::CreateIdentity();
	view_width = (float)launcher->GetTargetWidth();
	view_height = (float)launcher->GetTargetHeight();
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
}

void Camera::SetViewWidth(float width){
	this->view_width = width;
	float scale = (float)launcher->GetTargetWidth() / view_width;
	view_height = (float)launcher->GetTargetHeight() / scale;
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
}

void Camera::SetPosition(Vector2D pos){
	view.SetTranslation(pos*(-1));
}

Matrix Camera::GetViewMatrix(){
	return view;
}

Matrix Camera::GetProjectionMatrix(){
	return projection;
}

float Camera::GetViewWidth(){
	return view_width;
}

float Camera::GetViewHeight(){
	return view_height;
}