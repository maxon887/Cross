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
	background = graphics->LoadImage("Background.jpg", game->GetScaleFactor() * bcg_scale);
	//spider_body = graphics->LoadImage("Spider/Body.png", game->GetScaleFactor() * 0.8f);
	//spider_head = graphics->LoadImage("Spider/Body.png", game->GetScaleFactor() * 0.8f);
	Image* image[8];
}

void SecondScreen::Update(float sec){
    graphics->Clear(1, 1, 1);
    if(input->HaveInput()){
        game->SetScreen(new MainScreen(game));
    }
	DrawBackground(sec);
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
	float x = background->GetWidth() / 2;
	float y = background->GetHeight() / 2;
	if(run_time > 0)
		deltaY = deltaY + sec * 500.0f;
	y -= deltaY;
	//launcher->LogIt(itoa(y, str_buffer, 10));
	if(deltaY > background->GetHeight()*bcg_scale){
		deltaY -= background->GetHeight()*bcg_scale;
	}
	while(y - background->GetHeight()/2 < launcher->GetTargetHeight() / game->GetScaleFactor()*bcg_scale) {
		while(x - background->GetWidth()/2 < game->GetWidth()*bcg_scale) {
			graphics->DrawImage(x, y, background);
			x += background->GetWidth() * bcg_scale;
		}
		x = background->GetWidth() / 2;
		y += background->GetHeight() * bcg_scale;
	}
}