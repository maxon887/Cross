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
#include "ToggleButton.h"
#include "System.h"
#include "Graphics2D.h"
#include "Utils/PrimitiveDrawer.h"

using namespace cross;

ToggleButton::ToggleButton(Sprite* on, Sprite* off)
	:Button()
{
	SetImages(on, off);
	this->on = on;
	this->off = off;
	Clicked.Connect(this, &ToggleButton::OnClicked);
}

ToggleButton::ToggleButton(Vector2D location, Sprite* on, Sprite* off)
	:Button(location)
{
	this->SetImages(on, off);
	this->on = on;
	this->off = off;
	Clicked.Connect(this, &ToggleButton::OnClicked);
}

void ToggleButton::Update(){
	CROSS_FAIL(located, "Button must be located first")

	if(state)
		PrimitiveDrawer::DrawSprite(location, on);
	else
		PrimitiveDrawer::DrawSprite(location, off);
}

bool ToggleButton::GetState() const{
	return state;
}

void ToggleButton::SetState(bool state){
	this->state = state;
}

void ToggleButton::OnClicked(){
	state = !state;
}