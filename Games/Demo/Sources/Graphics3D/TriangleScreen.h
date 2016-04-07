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
#pragma once
#include "Cross.h"
#include "Graphics3D/CameraControlScreen.h"
#include "TriangleShaders.h"
#include "Input.h"

using namespace cross;

class TriangleScreen : public CameraControlScreen{
public:
	void Start();
	void Stop();
	void Update(float sec);
private:
	Font* debug_font;
	GLuint vboId;
	TriangleShaders* shader;

	Vector2D touch_pos;

	FastDelegate1<Input::Action, void> action_move_delegate;
	void ActionMoveHandle(Input::Action action);
};