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
#include "Eatable.h"
#include "Animation.h"

class Snake : public Collisioner{
public:
	static void Init();
	static void Release();

	Snake();
	~Snake();

	float GetRadius();
	void CollisionOccurred(Collisioner* obj);

	float Direction();
	void Rotate(float angle);
	void Update(float sec);
	void Draw();
	float GetSpeedW();
	vector<Body*>& GetBodyNodes();
	Collisioner* GetRadar();
private:
	class Radar : public Collisioner{
	public:
		Radar(Point pos, Snake* snake):Collisioner(pos), snake(snake){};
		float GetRadius(){ return 60.f; }
		void CollisionOccurred(Collisioner* obj);
		void ObjectLeft(Collisioner* obj);
		Snake* snake;
	};
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
	float star_angle;
	list<Point> body_path;
	vector<Body*> body_nodes;
	list<pair<Eatable*, float>> eatables;
	Radar* radar;

	bool OnBorder();
	void UpdateBody(float sec);
	void DrawBody();
	void Die();
};
