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
	
#include "TestScreen.h"
#include "Game.h"
#include "Vector2D.h"
#include "Graphics2D.h"

void TestScreen::Start(){
	going_back = false;
	input->KeyPressed += MakeDelegate(this, &TestScreen::OnKeyPressed);
	
	Vector2D v1(-5, 5);
	Vector2D v2(0, -10);
	Vector2D v3(13, 7);
	Vector2D v4(-4, 3);
	Vector2D resV;
	resV = v1 + v2 + v3 + v4;
	float magnityde = resV.Length();
	resV.Normalize();

	Vector2D H(1, 0);
	Vector2D P(4, 5);
	P.Normalize();
	float dotProd = H.DotProduct(P);
	float angle = acos(dotProd);
}

void TestScreen::Update(float sec){
	gfx2D->Clear();

	if(going_back){
		game->SetScreen(game->GetStartScreen());
	}
}

void TestScreen::OnKeyPressed(Key key){
	switch (key)
	{
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