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
#include "Screen.h"
#include "Input.h"

using namespace cross;

class ScrollScreen : public Screen{
public:
	void Start() override;
	void Stop() override;

	float GetWidth() override;
	float GetHeight() override;

	void SetWidth(float width);
	void SetHeight(float height);

	void ResetScreenSize();

private:
	float width;
	float height;
	Vector2D touch;

	void ActionDown(Input::Action action) override;
	void ActionMove(Input::Action action) override;
	void ActionUp(Input::Action action) override;

};