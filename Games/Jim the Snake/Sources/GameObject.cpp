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
	
#include "GameObject.h"

JimTheSnake*		GameObject::game = NULL;
GameScreen*			GameObject::screen = NULL;
Graphics*			GameObject::graphics = NULL;
int					GameObject::count = 0;

void GameObject::Init(JimTheSnake* game){
	GameObject::game = game;
	GameObject::graphics = game->graphics;
	GameObject::screen = dynamic_cast<GameScreen*>(game->GetCurrentScreen());
	if(!screen){
		throw string("Game object must be initialized under GameScreen");
	}
}

GameObject::GameObject(){
	Initialize(Point(0, 0), 0);
}

GameObject::GameObject(Point pos){
	Initialize(pos, 0);
}

GameObject::GameObject(Point pos, int layer){
	Initialize(pos, layer);
}

GameObject::~GameObject(){
	count--;
}

int GameObject::GetId(){
	return id;
}

void GameObject::SetPosition(Point pos){
	this->position = pos;
}

Point GameObject::GetPosition(){
	return position;
}
/*
bool GameObject::operator < (GameObject& obj){
	return id < obj.GetId();
}*/

void GameObject::Initialize(Point pos, int layer){
	id = count + layer * 1000;
	count++;
	SetPosition(pos);
}
