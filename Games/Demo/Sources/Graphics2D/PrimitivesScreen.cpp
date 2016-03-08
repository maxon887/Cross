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
#include "Input.h"
#include "Graphics2D.h"
#include "Game.h"
#include "Launcher.h"
#include "Utils/Misc.h"
#include "Sprite.h"

void PrimitivesScreen::Start(){
	generate_entities = false;
	input->ActionDown += MakeDelegate(this, &PrimitivesScreen::OnActionDown);
	input->ActionMove += MakeDelegate(this, &PrimitivesScreen::OnActionMove);
	input->ActionUp += MakeDelegate(this, &PrimitivesScreen::OnActionUp);

	//awsome_face = gfx2D->LoadImage("AwesomeFace.png");
	//awsome_face->SetRotate(45.f);
	//awsome_face->SetPosition(Vector2D(game->GetWidth() / 2.f, game->GetHeight() / 2.f));
}

void PrimitivesScreen::Stop(){
	input->ActionDown.RemoveDelegate(input->ActionDown.GetLastDelegate());
	input->ActionMove.RemoveDelegate(input->ActionMove.GetLastDelegate());
	input->ActionUp.RemoveDelegate(input->ActionUp.GetLastDelegate());
	for(auto iter = entities.begin(); iter != entities.end();) {
		Entity* entity = (*iter);
		delete entity;
		iter = entities.erase(iter);
	}
}

void PrimitivesScreen::Update(float sec){
	//static float rot = 0;
	//rot += sec * 14;
	//awsome_face->SetRotate(rot);
	//awsome_face->SetScale(0.1f);
	//gfx2D->DrawSprite(awsome_face);

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

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)){
		game->SetScreen(game->GetStartScreen());
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

void PrimitivesScreen::OnActionDown(Vector2D pos){
	generate_entities = true;
	spawn = pos;
}

void PrimitivesScreen::OnActionMove(Vector2D pos){
	spawn = pos;
}

void PrimitivesScreen::OnActionUp(Vector2D pos){
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
	gfx2D->DrawPoint(position, color);
}

bool PrimitivesScreen::Point::IsVisible(){
	if(position.x < game->GetWidth() && position.x >= 0 &&
		position.y < game->GetHeight() && position.y >= 0){
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
	gfx2D->DrawCircle(position, radius, color);
}

bool PrimitivesScreen::Circle::IsVisible(){
	if(position.x + radius < game->GetWidth() && position.x + radius >= 0 &&
		position.y + radius < game->GetHeight() && position.y + radius >= 0){
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
	gfx2D->DrawLine(position, second_pos, color);
}

bool PrimitivesScreen::Line::IsVisible(){
	bool firstVisible = false;
	if(position.x < game->GetWidth() && position.x >= 0 &&
		position.y < game->GetHeight() && position.y >= 0) {
		firstVisible = true;
	} else {
		firstVisible = false;
	}

	bool secondVisible = false;
	if(second_pos.x < game->GetWidth() && second_pos.x >= 0 &&
		second_pos.y < game->GetHeight() && second_pos.y >= 0) {
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
	gfx2D->DrawRect(Rect(position, width, height), color);
}

bool PrimitivesScreen::Rectangle::IsVisible(){
	if(position.x < game->GetWidth() && position.x >= 0 &&
		position.y < game->GetHeight() && position.y >= 0) {
		return true;
	} else {
		return false;
	}
}