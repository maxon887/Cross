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
#include "Graphics2D.h"
#include "BirdsScreen.h"
#include "Game.h"
#include "Bird.h"

void BirdsScreen::Start(){
	going_back = false;
	cursor.x = game->GetWidth() / 2;
	cursor.y = game->GetHeight() / 2;
	bird = new Bird(0.5f, 150, 1, 1);
	bird->pos = Vector2D(200, 200);
	bird->velocity = Vector2D(20, 20);
	input->KeyPressed += MakeDelegate(this, &BirdsScreen::OnKeyPressed);
	input->ActionDown += MakeDelegate(this, &BirdsScreen::OnActionDown);
}

void BirdsScreen::Update(float sec){
	gfx2D->Clear();
	DrawCursor(cursor);
	bird->Update(sec, cursor);
	bird->Draw();
	if(going_back){
		game->SetScreen(game->GetStartScreen());
	}
}

void BirdsScreen::DrawCursor(Vector2D pos){
	//graphics->DrawLine(Vector2D(pos.x, pos.y + 5), Vector2D(pos.x, pos.y - 6), Color::Red);
	//graphics->DrawLine(Vector2D(pos.x + 5, pos.y), Vector2D(pos.x - 6, pos.y), Color::Red);
	//graphics->DrawCircle(Vector2D(pos.x, pos.y), 3.f, Color::Red);
}

void BirdsScreen::OnActionDown(Vector2D pos){
	cursor.x = pos.x;
	cursor.y = pos.y;
}

void BirdsScreen::OnKeyPressed(Key key){
	switch (key) {
	case cross::Key::UNDEFINED:
		break;
	case cross::Key::PAUSE:
		break;
	case cross::Key::BACK:
		break;
	case cross::Key::OPTIONS:
		break;
	case cross::Key::UP:
		break;
	case cross::Key::DOWN:
		break;
	case cross::Key::RIGHT:
		break;
	case cross::Key::LEFT:
		break;
	case cross::Key::ENTER:
		break;
	case cross::Key::SPACE:
		break;
	case cross::Key::SHIFT:
		break;
	case cross::Key::ESCAPE:
		going_back = true;
		break;
	default:
		break;
	}
}