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
	
#include "MenuScreen.h"

MenuScreen::MenuScreen(Game* game):Screen(game) { }

void MenuScreen::Start(){
	Screen::Start();
}

void MenuScreen::Update(float sec){
	//graphics->Test();
	graphics->Clear(0.0f, 0.25f, 0.25f);
	graphics->Test();
}