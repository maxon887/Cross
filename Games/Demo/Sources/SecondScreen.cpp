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
	deltaY = 0;
	turn_left = true;
}

void SecondScreen::Start(){
	spider_body = graphics->LoadImage("Spider/Body.png", game->GetScaleFactor());
	spider_head = graphics->LoadImage("Spider/Head.png", game->GetScaleFactor());
	background = graphics->LoadRepeatedImage("Background.jpg", game->GetScaleFactor() * bcg_scale, 900, 3000);
	Image* image[8];
}

void SecondScreen::Update(float sec){
    graphics->Clear(0, 0, 0);
    if(input->HaveInput()){
        game->SetScreen(new MainScreen(game));
    }
	DrawBackground(sec);
	//graphics->DrawImage(0, 0, background);

	if(run_time > 0) {
		run_time -= sec;
		//graphics->DrawImage(game->GetWidth() / 2, game->GetHeight() / 2, spider_run.GetImage());
		if(run_time < 0)
			thinking_time = 1.3f;
	}
	if(thinking_time > 0){
		thinking_time -= sec;
			if(turn_left) {
				head_angle += sec * 150.f;
				if(head_angle > 45)
					turn_left = false;
			} else {
				head_angle -= sec * 150.f;
				if(head_angle < -45)
					turn_left = true;
			}

			if(thinking_time < 0){
				run_time = 3.5f;
				head_angle = 0;
			}
	}
}

void SecondScreen::DrawBackground(float sec) {
	float y = background->GetHeight() / 2;
	if(run_time > 0)
		deltaY = deltaY + sec * 500.0f;
	y -= deltaY;
	
	if(deltaY > background->texHeight*bcg_scale){
		deltaY -= background->texHeight*bcg_scale;
	}
	graphics->DrawImage(0, y, background);
}
