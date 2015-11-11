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
#include "BirdsScreen.h"
#include "Game.h"
#include "Bird.h"

void BirdsScreen::Start(){
	going_back = false;
	cursor.x = game->GetWidth() / 2;
	cursor.y = game->GetHeight() / 2;
	bird = new Bird();
	input->KeyPressed += MakeDelegate(this, &BirdsScreen::OnKeyPressed);
	input->ActionDown += MakeDelegate(this, &BirdsScreen::OnActionDown);
}

void BirdsScreen::Update(float sec){
	graphics->Clear(0.f, 0.f, 0.f);
	DrawCursor(cursor);
	if(going_back){
		game->SetScreen(game->GetStartScreen());
	}
}

void BirdsScreen::DrawCursor(Vector2D pos){
	graphics->DrawPixel(Point(0, 0), Color::Red);
	graphics->DrawPixel(Point(1, 1), Color::Red);
	graphics->DrawPixel(Point(0, 1), Color::Red);
	graphics->DrawPixel(Point(1, 0), Color::Red);
	graphics->DrawLine(Point(pos.x, pos.y + 5), Point(pos.x, pos.y - 6), Color::Red);
	graphics->DrawLine(Point(pos.x + 5, pos.y), Point(pos.x - 6, pos.y), Color::Red);
	graphics->DrawCircle(Point(pos.x, pos.y), 3.f, Color::Red);
}

void BirdsScreen::OnActionDown(Point pos){
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