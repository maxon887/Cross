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

#include "Game.h"
#include "Launcher.h"
#include "Graphics.h"
#include "Screen.h"
#include "Debuger.h"

Game::Game(Launcher* launcher, float width){
	this->launcher = launcher;
	this->width = width;
	float aspect = (float)launcher->GetTargetHeight() / (float)launcher->GetTargetWidth();
	height = width * aspect;
	scale_factor = (float)launcher->GetTargetWidth() / width;
}

float Game::GetScaleFactor(){
	return scale_factor;
}

float Game::GetWidth(){
	return width;
}

float Game::GetHeight(){
	return height;
}

void Game::Start(){
#ifdef CROSSDEBUG
    debuger = new Debuger(this);
#endif
	SetScreen(GetStartScreen());
}

void Game::SetScreen(Screen* screen){
	current_screen = screen;
	current_screen->Init();
	current_screen->Start();
}

Screen* Game::GetCurrentScreen(){
	return current_screen;
}

Game::~Game(){ }


void Game::Update(){
    clock_t rend = clock() - render_time;
    render_time = clock();
    GetCurrentScreen()->Update(rend / (float)CLOCKS_PER_SEC);
#ifdef CROSSDEBUG
    debuger->Display(rend / (float)CLOCKS_PER_SEC);
    clock_t up = clock() - render_time;
    debuger->SetUpdateTime(up / (float)CLOCKS_PER_SEC);
#endif
}