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
#include "Menu.h"
#include "Game.h"
#include "Sprite.h"
#include "Launcher.h"
#include "Screen.h"

Menu::Menu():
	button_height(0),
	button_width(0)
{
	window_resized_delegate = MakeDelegate(this, &Menu::WindowResizedHandle);
	launcher->WindowResized += window_resized_delegate;
}

Menu::~Menu(){
	Clear();
	launcher->WindowResized -= window_resized_delegate;
}

void Menu::Update(float sec){
	float height = game->GetCurrentScreen()->GetHeight();
	Vector2D pos(game->GetCurrentScreen()->GetWidth() / 2.0f, height - (offset - button_height/2));
	for(Button* btn : buttons){
		btn->SetLocation(pos);
		btn->Update();
		pos.y -= offset;
	}
}

void Menu::Active(bool active){
	for(Button* btn : buttons){
		btn->SetActive(active);
	}
}

void  Menu::AddButton(Button* but){
	buttons.push_back(but);
	if(button_width == 0){
		button_def_width = but->GetWidth();
		button_def_height = but->GetHeight();
		button_width = but->GetWidth();
		button_height = but->GetHeight();
	}
	if(button_def_height != but->GetHeight() && button_def_width != but->GetWidth()){
		throw CrossException("All buttons must be equal size");
	}
}

int Menu::Count(){
	return buttons.size();
}

void  Menu::Clear(){
	for(Button* btn : buttons){
		delete btn;
	}
	buttons.clear();
	button_def_width = 0;
	button_def_height = 0;
	button_width = 0;
	button_height = 0;
	offset = 0;
}

void Menu::LocateButtons(){
	float height = game->GetCurrentScreen()->GetHeight();
	int devider = buttons.size() + 1;
	offset = (height + button_height) / devider;

	if(offset < (button_height + 20)){
		float coef = offset / (button_def_height + 20);
		for(Button* btn : buttons){
			btn->Scale(coef);
		}
		button_width = buttons[0]->GetWidth();
		button_height = buttons[0]->GetHeight();
	}
}

void Menu::WindowResizedHandle(int w, int h){
	float height = game->GetCurrentScreen()->GetHeight();
	int devider = buttons.size() + 1;
	offset = (height + button_height) / devider;

	if(offset < (button_height + 20)){
		float coef = offset / (button_def_height + 20);
		for(Button* btn : buttons){
			btn->Scale(coef);
		}
	}
	button_width = buttons[0]->GetWidth();
	button_height = buttons[0]->GetHeight();
}