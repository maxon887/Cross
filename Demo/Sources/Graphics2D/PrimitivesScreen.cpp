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
#include "PrimitivesScreen.h"
#include "Utils/PrimitiveDrawer.h"
#include "Game.h"

void PrimitivesScreen::Start(){
	Screen::Start();
	generate_entities = false;
	SetBackground(Color::Black);
}

void PrimitivesScreen::Stop(){
	for(auto iter = entities.begin(); iter != entities.end();) {
		Entity* entity = (*iter);
		delete entity;
		iter = entities.erase(iter);
	}
	Screen::Stop();
}

void PrimitivesScreen::Update(float sec){
	Screen::Update(sec);
	if(generate_entities){
		GenerateEntity();
	}

	for(auto iter = entities.begin(); iter != entities.end();) {
		Entity* entity = (*iter);
		if(!entity->IsVisible()){
			delete entity;
			iter = entities.erase(iter);
			continue;
		}else{
			iter++;
		}
		entity->Update(sec);
		entity->Draw();
	}
}

void PrimitivesScreen::GenerateEntity(){
	Entity* entity = nullptr;
	switch(entity_type)
	{
	case PrimitivesScreen::Entity::POINT:
		entity = new Point(spawn);
		break;
	case PrimitivesScreen::Entity::CIRCLE:
		entity = new Circle(spawn);
		break;
	case PrimitivesScreen::Entity::LINE:
		entity = new Line(spawn);
		break;
	case PrimitivesScreen::Entity::RECTANGLE:
		entity = new Rectangle(spawn);
		break;
	case PrimitivesScreen::Entity::NONE:
		break;
	default:
		break;
	}
	entities.push_back(entity);
}

void PrimitivesScreen::ActionDown(Input::Action action){
	generate_entities = true;
	spawn = action.pos;
}

void PrimitivesScreen::ActionMove(Input::Action action){
	spawn = action.pos;
}

void PrimitivesScreen::ActionUp(Input::Action action){
	generate_entities = false;
	entity_type = (Entity::Type)((entity_type+1) % Entity::Type::NONE);
}

PrimitivesScreen::Entity::Entity(Vector2D pos) :
	color(Color::Random()),
	position(pos)
{
	velocity.x = Random(-50.f, 50.f);
	velocity.y = Random(50.f, 100.f);
}

void PrimitivesScreen::Entity::Update(float sec){
	position += velocity * sec;
}

PrimitivesScreen::Point::Point(Vector2D pos) :
	Entity(pos)
{ }

void PrimitivesScreen::Point::Draw(){
	PrimitiveDrawer::DrawPoint(position, color);
}

bool PrimitivesScreen::Point::IsVisible(){
	if(position.x < game->GetCurrentScreen()->GetWidth() && position.x >= 0 &&
		position.y < game->GetCurrentScreen()->GetHeight() && position.y >= 0){
		return true;
	}else{
		return false;
	}
}

PrimitivesScreen::Circle::Circle(Vector2D pos) :
	Entity(pos)
{
	radius = Random(5.f, 15.f);
}

void PrimitivesScreen::Circle::Draw(){
	PrimitiveDrawer::DrawCircle(position, radius, color);
}

bool PrimitivesScreen::Circle::IsVisible(){
	if(position.x + radius < game->GetCurrentScreen()->GetWidth() && position.x + radius >= 0 &&
		position.y + radius < game->GetCurrentScreen()->GetHeight() && position.y + radius >= 0){
		return true;
	}else{
		return false;
	}
}

PrimitivesScreen::Line::Line(Vector2D pos) : 
	Entity(pos)
{
	second_pos.x = position.x + Random(-10.f, 10.f);
	second_pos.y = position.y + Random(10.f, 10.f);
	second_vel.x = Random(-50.f, 50.f);
	second_vel.y = Random(50.f, 100.f);
}

void PrimitivesScreen::Line::Draw(){
	PrimitiveDrawer::DrawLine(position, second_pos, color);
}

bool PrimitivesScreen::Line::IsVisible(){
	bool firstVisible = false;
	if(position.x < game->GetCurrentScreen()->GetWidth() && position.x >= 0 &&
		position.y < game->GetCurrentScreen()->GetHeight() && position.y >= 0) {
		firstVisible = true;
	} else {
		firstVisible = false;
	}

	bool secondVisible = false;
	if(second_pos.x < game->GetCurrentScreen()->GetWidth() && second_pos.x >= 0 &&
		second_pos.y < game->GetCurrentScreen()->GetHeight() && second_pos.y >= 0) {
		secondVisible = true;
	} else {
		secondVisible = false;
	}

	if(firstVisible || secondVisible){
		return true;
	}else{
		return false;
	}
}

void PrimitivesScreen::Line::Update(float sec){
	Entity::Update(sec);
	second_pos += second_vel * sec;
}

PrimitivesScreen::Rectangle::Rectangle(Vector2D pos) : 
	Entity(pos)
{
	width = Random(10.f, 50.f);
	height = Random(10.f, 50.f);
}

void PrimitivesScreen::Rectangle::Draw(){
	PrimitiveDrawer::DrawRect(Rect(position, width, height), color);
}

bool PrimitivesScreen::Rectangle::IsVisible(){
	if(position.x < game->GetCurrentScreen()->GetWidth() && position.x >= 0 &&
		position.y < game->GetCurrentScreen()->GetHeight() && position.y >= 0) {
		return true;
	} else {
		return false;
	}
}