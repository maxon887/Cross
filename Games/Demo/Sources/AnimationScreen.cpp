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
	
#include "AnimationScreen.h"
#include "MainScreen.h"

void AnimationScreen::Start(){
	bcg_scale = 2;
	run_time = 2;
	thinking_time = 0;
	head_angle = 0;
	deltaY = 0;
	turn_left = true;
	going_back = false;
	input->KeyPressed += MakeDelegate(this, &AnimationScreen::OnKeyPressed);
	spider_body = graphics->LoadImage("Spider/Body.png", game->GetScaleFactor() * 0.8f);
	spider_head = graphics->LoadImage("Spider/Head.png", game->GetScaleFactor() * 0.8f);
	background = graphics->LoadRepeatedImage("Background.jpg", 900, 3000, game->GetScaleFactor() * bcg_scale);
	Image* images[8];
	images[0] = graphics->LoadImage("Spider/00.png", game->GetScaleFactor() * 0.8f);
	images[1] = graphics->LoadImage("Spider/01.png", game->GetScaleFactor() * 0.8f);
	images[2] = graphics->LoadImage("Spider/02.png", game->GetScaleFactor() * 0.8f);
	images[3] = graphics->LoadImage("Spider/03.png", game->GetScaleFactor() * 0.8f);
	images[4] = graphics->LoadImage("Spider/04.png", game->GetScaleFactor() * 0.8f);
	images[5] = graphics->LoadImage("Spider/05.png", game->GetScaleFactor() * 0.8f);
	images[6] = graphics->LoadImage("Spider/06.png", game->GetScaleFactor() * 0.8f);
	images[7] = graphics->LoadImage("Spider/07.png", game->GetScaleFactor() * 0.8f);
	spider_run_anim = new Animation(graphics, 0.08f, images, 8, true);
	spider_run_snd = new Audio("SpiderRun.wav", true, false);
}

void AnimationScreen::Update(float sec){
    graphics->Clear(0, 0, 0);
	DrawBackground(sec);
	spider_run_anim->Update(sec);

	if(run_time >= 0) {
		run_time -= sec;
		graphics->DrawImage(game->GetWidth() / 2, game->GetHeight() / 2, spider_run_anim->GetImage());
		if(run_time < 0){
			thinking_time = 1.3f;
			spider_run_snd->Stop();
		}
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
			spider_run_snd->Play();
		}
	}
	if(going_back){
		game->SetScreen(game->GetStartScreen());
	}
}

void AnimationScreen::DrawBackground(float sec) {
	float y = background->GetHeight() / 2;
	if(run_time > 0)
		deltaY = deltaY + sec * 430.0f;
	y -= deltaY;
	
	if(deltaY > background->texHeight*bcg_scale){
		deltaY -= background->texHeight*bcg_scale;
	}
	graphics->DrawImage(0, y, background);
}

void AnimationScreen::OnKeyPressed(Key key){
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

AnimationScreen::~AnimationScreen(){
	delete spider_run_anim;
	delete spider_run_snd;
	graphics->ReleaseImage(background);
	graphics->ReleaseImage(spider_body);
	graphics->ReleaseImage(spider_head);
}
