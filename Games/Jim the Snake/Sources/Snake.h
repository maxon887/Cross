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
#include "SnakyGame.h"
#include "Animation.h"

enum SnakeState{
	ALIVE,
	DEAD0,
	DEAD1
};

class Snake{
public:
	const float face_radius = 26.f;
	const float body_radius = 26.f;
	const float speedV = 240.f;
	const float speedW = speedV / 1.33f;

	Snake(Game* game);
private:
	Graphics* graphics;

	SnakeState state;
	Animation* face_bottom_anim;
	Animation* face_top_anim;
	Image* face_dead;
	float face_angle;
	PointX pos;
	vector<PointX> body_path;
	vector<PointX> body_nodes;
	float body_length;
	const float nod_length = 50.f;
	Image* body_img;
	vector<int> big_nodes;
	float body_time_left;
	float apple_time_left;
	Audio* eat_snd;
	Image* star_img;
	const float star_speedW = 2.f;

};