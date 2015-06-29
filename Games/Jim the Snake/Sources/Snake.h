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
#include "JimTheSnake.h"
#include "Apple.h"
#include "Spider.h"
#include "Animation.h"
#include "Eatable.h"

class Body : public GameObject{
public:
	static Image* img;
	Body(Point pos):GameObject(pos){
		big = false;
	}
	float GetRadius(){
		return 26.f;
	}
	void Draw(){
		if(!big){
			graphics->ScaleImage(img, game->GetScaleFactor());
		}else{
			graphics->ScaleImage(img, game->GetScaleFactor() * 1.2f);
		}
		graphics->DrawImage(position, img);
	}
	bool big;
};

class Snake : public GameObject{
public:
	static void Init();
	static void Release();
	
	Snake();
	float GetSpeedW();
	float Direction();
	void Rotate(float angle);
	float GetRadius();
	void Update(float sec);
	void Draw();
private:
	static const float speedV;
	static const float speedW;
	static const float nod_length;
	static Animation* bottom_anim;
	static Animation* top_anim;
	static Image* dead_face;
	static Image* star_img;
	float angle;
	bool dead;

	vector<Point> body_path;
	vector<Body*> body_nodes;
	float body_length;
	/*
	float face_angle;
	Snake();
	int GetScore();
	bool OnCollision(Point center, float radius);
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
	enum SnakeState{
		ALIVE,
		DEAD0,
		DEAD1
	};
	static JimTheSnake* game;
	static Graphics* graphics;

	//static const float face_radius;
	//static const float body_radius;
	static const float speedV;
	static const float speedW;
	static const float nod_length;
	static const float star_speedW;
	static Image* body_img;
	static Image* star_img;

	static Audio* eat_snd;

	void CalcBigNodes(float sec);

	SnakeState state;
	Head* head;
	int score;
	//Point face_pos;
	vector<Point> body_path;
	vector<Body> body_nodes;
	float body_length;
	list<int> big_nodes;
	float body_time_left;
	float eatable_time_left;
	float star_angle;
	Eatable* near_eatable;*/
};
