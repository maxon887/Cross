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
#include "GameObject.h"
#include "Game.h"

using namespace cross;

class Cactus : public GameObject{
public:
	static void Init();
	static void Release();

	Cactus();

	float GetRadius();

	void Update(float sec);
	void Draw();
	bool IsDead();
private:
	enum CactusState {
		GROWING,
		ADULT,
		OLD,
		HIDING,
		EMPTY
	};

	static Image* small_img;
	static Image* adult_img;
	static Image* old_img;

	CactusState state;
	float life_time;
	float lerp;
	float shake_state;
	float scale_factor;

	bool shake_up;
};