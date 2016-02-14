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
	float width = (float)launcher->GetTargetWidth();
	float height = (float)launcher->GetTargetHeight();
	projection = Matrix::CreateOrthogonalProjection(0, width, 0, height, 1, -1);
}

void Camera::ViewWidth(float width){
	float scale = (float)launcher->GetTargetWidth() / width;
	float height = (float)launcher->GetTargetHeight() / scale;
	projection = Matrix::CreateOrthogonalProjection(0, width, 0, height, 1, -1);
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