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
#include "Apple.h"
#include "Spider.h"
#include "Animation.h"
#include "Eatable.h"

enum class SnakeState{
	ALIVE,
	DEAD0,
	DEAD1
};

class Snake{
public:
	static void Init(JimTheSnake* game);
	static void Release();
	float face_angle;
	Snake();
	int GetScore();
	bool OnCollision(PointX center, float radius);
	void EatableNear(Eatable* eatable);
	void DrawFace(float sec);
	void DrawFaceDeadST0();
	void DrawFaceDeadST1(float sec);
	void CalcHead(float sec);
	void DrawBody(float sec);
	bool OnBiteYouself();
	bool OnBorder();
	float GetSpeedW();
private:
	static JimTheSnake* game;
	static Graphics* graphics;

	static const float face_radius;
	static const float body_radius;
	static const float speedV;
	static const float speedW;
	static const float nod_length;
	static const float star_speedW;

	static Animation* face_bottom_anim;
	static Animation* face_top_anim;
	static Image* face_dead;
	static Image* body_img;
	static Image* star_img;

	static Audio* eat_snd;

	void CalcBigNodes(float sec);

	SnakeState state;
	int score;
	PointX face_pos;
	vector<PointX> body_path;
	vector<PointX> body_nodes;
	float body_length;
	list<int> big_nodes;
	float body_time_left;
	float eatable_time_left;
	float star_angle;
	Eatable* near_eatable;
};
