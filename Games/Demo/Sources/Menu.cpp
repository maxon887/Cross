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

Menu::Menu(bool resizeble):
	button_height(0),
	button_width(0),
	located(false),
	resizeble(resizeble)
{
	window_resized_delegate = MakeDelegate(this, &Menu::WindowResizedHandle);
	game->WindowResized += window_resized_delegate;
}

Menu::~Menu(){
	Clear();
	game->WindowResized -= window_resized_delegate;
}

void Menu::Update(float sec){
	if(!located){
		Locate();
	}
	for(Button* btn : buttons){
		btn->Update(sec);
	}
}

void Menu::Active(bool active){
	for(Button* btn : buttons){
		btn->SetActive(active);
	}
}

void Menu::AddButton(Button* but){
	buttons.push_back(but);
}

void Menu::SetImages(Sprite* up, Sprite* down){
	for(Button* btn : buttons){
		btn->SetImages(up->Clone(), down->Clone());
	}
	if(button_width == 0){
		button_def_width = buttons[0]->GetWidth();
		button_def_height = buttons[0]->GetHeight();
		button_width = buttons[0]->GetWidth();
		button_height = buttons[0]->GetHeight();
	}
}

U32 Menu::Count(){
	return buttons.size();
}

void Menu::Clear(){
	for(Button* btn : buttons){
		delete btn;
	}
	buttons.clear();
	button_def_width = 0;
	button_def_height = 0;
	button_width = 0;
	button_height = 0;
	offset = 0;
	located = false;
}

float Menu::GetHeight(){
	if(!located){
		Locate();
	}
	return menu_height;
}

void Menu::Locate(){
	if(resizeble){
		menu_height = game->GetCurrentScreen()->GetHeight();
		S32 devider = buttons.size() + 1;
		offset = (menu_height + button_height) / devider;

		Vector2D pos(game->GetCurrentScreen()->GetWidth() / 2.f, menu_height - (offset - button_height/2.f));

		for(Button* btn : buttons){
			if(offset < (button_height + 20.f)){
				float coef = offset / (button_def_height + 20);
				btn->Scale(coef);
			}

			btn->SetLocation(pos);
			//launcher->LogIt("Pos x - %f, y - %f", pos.x, pos.y);
			pos.y -= offset;
		}
		button_width = buttons[0]->GetWidth();
		button_height = buttons[0]->GetHeight();
	}else{
		static const float space_len = 25.f;
		menu_height = button_height * Count() + Count() * space_len;
		Vector2D pos(game->GetCurrentScreen()->GetWidth() / 2.f, menu_height - button_height/2.f - space_len/2.f);
		for(Button* btn : buttons){
			btn->SetLocation(pos);
			pos.y -= button_height + space_len;
		}
	}
	located = true;
}

void Menu::WindowResizedHandle(S32 w, S32 h){
	Locate();
}
