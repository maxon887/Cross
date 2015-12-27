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
#include "Screen.h"
#include "Input.h"

using namespace cross;
using namespace glm;

Camera::Camera():
	pos(0.0f, 0.0f, 3.0f),
	direction(0.0f, 0.0f, -1.0f),
	up(0.0f, 1.0f, 0.0f),
	speed(2.0f){
	matrix = lookAt(pos, pos + direction, up);
}

void Camera::Update(float sec){
	static float pitch = 0;
	static float yaw = 0;
	if(input->IsPressed(Key::SHIFT)){
		pos += speed * direction * sec;
	}
	if(input->IsPressed(Key::CONTROL)){
		pos -= speed * direction * sec;
	}
	if(input->IsPressed(Key::W)){
		pitch -= sec * 30.0f;
	}
	if(input->IsPressed(Key::A)){
		//pos -= glm::normalize(glm::cross(direction, up)) * speed * sec;
		yaw -= sec * 30.0f;
	}
	if(input->IsPressed(Key::S)){
		pitch += sec * 30.0f;
	}
	if(input->IsPressed(Key::D)){
		//pos += glm::normalize(glm::cross(direction, up)) * speed * sec;
		yaw += sec * 30.0f;
	}

	direction.z = -cos(radians(pitch)) * cos(radians(yaw));
	direction.y = sin(radians(pitch));
	direction.x = cos(radians(pitch)) * sin(radians(yaw));

	matrix = lookAt(pos, pos + direction, up);
}

float* Camera::GetMatrix(){
	return value_ptr(matrix);
}
