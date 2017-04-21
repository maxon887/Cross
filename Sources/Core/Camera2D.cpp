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
#include "Camera2D.h"
#include "Game.h"
#include "System.h"
#include "Screen.h"

using namespace cross;

Camera2D::Camera2D() {
	view_width = (float)system->GetWindowWidth();
	view_height = (float)system->GetWindowHeight();
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
    resize_del = cross::system->WindowResized.Connect(this, &Camera2D::WindowResizedHandle);
}

Camera2D::~Camera2D() {
	cross::system->WindowResized.Disconnect(resize_del);
}

void Camera2D::SetViewWidth(float width) {
	this->view_width = width;
	float scale = (float)system->GetWindowWidth() / view_width;
	view_height = (float)system->GetWindowHeight() / scale;
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
	//cross::system->WindowResized -= window_resized_delegate;
	//window_resized_delegate = MakeDelegate(this, &Camera2D::WindowRisezedHandleCust);
	//cross::system->WindowResized += window_resized_delegate;
}

float Camera2D::GetViewWidth() {
	return view_width;
}

float Camera2D::GetViewHeight() {
	return view_height;
}

void Camera2D::WindowResizedHandle(S32 width, S32 height) {
	view_width = (float)system->GetWindowWidth();
	view_height = (float)system->GetWindowHeight();
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
}

//do we really need this method?
void Camera2D::WindowRisezedHandleCust(S32 width, S32 height) {
	system->LogIt("TargetWidth - %d", system->GetWindowWidth());
	float scale = (float)system->GetWindowWidth() / view_width;
	view_height = (float)system->GetWindowHeight() / scale;
	projection = Matrix::CreateOrthogonalProjection(0, view_width, 0, view_height, 1, -1);
}