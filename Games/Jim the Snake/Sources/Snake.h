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
#include "JimTheSnake.h"
#include "Body.h"
#include "Apple.h"
#include "Spider.h"
#include "Animation.h"
#include "Eatable.h"

class Snake : public GameObject{
public:
	static void Init();
	static void Release();

	Snake();
	~Snake();

	float GetRadius();

	float Direction();
	void Rotate(float angle);
	void Update(float sec);
	void Draw();
	bool OnCollision(Point p);
	bool OnCollision(Point center, float radius);
	float GetSpeedW();
private:
	static const float speedV;
	static const float speedW;
	static const float nod_length;
	static const float star_speedW;

	static Animation* face_bottom_anim;
	static Animation* face_top_anim;
	static Image* face_dead;
	static Image* star_img;

	static Audio* eat_snd;
	static Audio* punch;

	bool dead;
	float angle;
	float body_length;
	float dead_time;
	float eatable_time_left;
	float star_angle;
	Eatable* near_eatable;
	vector<Point> body_path;
	vector<Body*> body_nodes;

	bool OnBiteYouself();
	bool OnBorder();
	void UpdateBody(float sec);
	void DrawBody();
	void EatableNear(Eatable* eatable);
};
