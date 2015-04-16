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

Image* images[8];
void SecondScreen::Start(){
	spider_body = graphics->LoadImage("Spider/Body.png", game->GetScaleFactor() * 0.8f);
	spider_head = graphics->LoadImage("Spider/Head.png", game->GetScaleFactor() * 0.8f);
	background = graphics->LoadRepeatedImage("Background.jpg", game->GetScaleFactor() * bcg_scale, 900, 3000);

	images[0] = graphics->LoadImage("Spider/00.png", game->GetScaleFactor() * 0.8f);
	images[1] = graphics->LoadImage("Spider/01.png", game->GetScaleFactor() * 0.8f);
	images[2] = graphics->LoadImage("Spider/02.png", game->GetScaleFactor() * 0.8f);
	images[3] = graphics->LoadImage("Spider/03.png", game->GetScaleFactor() * 0.8f);
	images[4] = graphics->LoadImage("Spider/04.png", game->GetScaleFactor() * 0.8f);
	images[5] = graphics->LoadImage("Spider/05.png", game->GetScaleFactor() * 0.8f);
	images[6] = graphics->LoadImage("Spider/06.png", game->GetScaleFactor() * 0.8f);
	images[7] = graphics->LoadImage("Spider/07.png", game->GetScaleFactor() * 0.8f);
	spider_run = new Animation(0.08f, images, 8, true);
}

void SecondScreen::Update(float sec){
    graphics->Clear(0, 0, 0);
    if(input->HaveInput()){
        game->SetScreen(new MainScreen(game));
    }
	DrawBackground(sec);
	spider_run->Update(sec);
	//graphics->DrawImage(0, 0, background);

	if(run_time >= 0) {
		run_time -= sec;
		graphics->DrawImage(game->GetWidth() / 2, game->GetHeight() / 2, spider_run->GetImage());
		if(run_time < 0)
			thinking_time = 1.3f;
	}
	if(thinking_time >= 0){
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
		graphics->Rotate(spider_head, head_angle);
		graphics->DrawImage(game->GetWidth() / 2, game->GetHeight() / 2, spider_body);
		graphics->DrawImage(game->GetWidth() / 2, game->GetHeight() / 2 + 115, spider_head);
		if(thinking_time < 0){
			run_time = 3.5f;
			head_angle = 0;
		}
	}
}

void SecondScreen::DrawBackground(float sec) {
	float y = background->GetHeight() / 2;
	if(run_time > 0)
		deltaY = deltaY + sec * 430.0f;
	y -= deltaY;
	
	if(deltaY > background->texHeight*bcg_scale){
		deltaY -= background->texHeight*bcg_scale;
	}
	graphics->DrawImage(0, y, background);
}
