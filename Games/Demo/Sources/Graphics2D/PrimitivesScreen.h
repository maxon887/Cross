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
#include "Screen.h"
#include "Font.h"

using namespace cross;

class PrimitivesScreen : public Screen{
public:
	void Start();
	void Stop();
	void Update(float sec);

private:
	class Entity{
	public:
		enum Type {
			POINT,
			CIRCLE,
			LINE,
			RECTANGLE,
			NONE
		};
		Vector2D velocity;
		Vector2D position;
		Color color;

		Entity(Vector2D pos);
		virtual ~Entity() { };

		virtual void Draw() = 0;
		virtual bool IsVisible() = 0;
		virtual void Update(float sec);
	};

	class Point : public Entity{
	public:
		Point(Vector2D pos);
		void Draw();
		bool IsVisible();
	};

	class Circle : public Entity{
	public:
		Circle(Vector2D pos);
		void Draw();
		bool IsVisible();
		float radius;
	};

	class Line : public Entity{
	public:
		Line(Vector2D pos);
		void Draw();
		bool IsVisible();
		void Update(float sec);
	private:
		Vector2D second_pos;
		Vector2D second_vel;
	};

	class Rectangle : public Entity{
	public:
		Rectangle(Vector2D pos);
		void Draw();
		bool IsVisible();
	private:
		float width;
		float height;
	};

	CRList<Entity*> entities;
	bool generate_entities;
	Vector2D spawn;
	Entity::Type entity_type;

	void GenerateEntity();

	void OnActionDown(Vector2D pos);
	void OnActionMove(Vector2D pos);
	void OnActionUp(Vector2D pos);
};