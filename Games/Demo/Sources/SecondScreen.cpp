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
	
#include "SecondScreen.h"
#include "MainScreen.h"

SecondScreen::SecondScreen(Game* game):Screen(game){
	bcg_scale = 2;
	run_time = 2;
	thinking_time = 0;
	head_angle = 0;
	turn_left = true;
}

void SecondScreen::Start(){
	background = graphics->LoadImage("Game/Background.jpg", game->GetScaleFactor() * bcg_scale);
	Image* image[8];
}

void SecondScreen::Update(float sec){
    graphics->Clear(0, 0, 0);
    if(input->HaveInput()){
        game->SetScreen(new MainScreen(game));
    }
}