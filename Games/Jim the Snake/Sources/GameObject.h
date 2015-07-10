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
#include "GameScreen.h"

class GameObject{
public:
	static void Init(JimTheSnake* game);

	virtual float GetRadius() = 0;
	virtual void OnCollision() { };
	virtual void Update(float sec) { };
	virtual void Draw() { };

	GameObject();
	GameObject(Point pos);
	GameObject(Point pos, int layer);
	virtual ~GameObject();

	void SetPosition(Point pos);
	int GetId();
	Point GetPosition();
	//bool operator < (GameObject& obj);
protected:
	static JimTheSnake* game;
	static GameScreen* screen;
	static Graphics* graphics;
	static int count;
	void Initialize(Point pos, int layer);
	Point position;
	int id;
};