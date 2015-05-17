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
#ifdef WIN
#include "LauncherWIN.h"
#endif 

Game::Game(Launcher* launcher, float width){
	this->launcher = launcher;
	this->input = new Input(this);
	this->saver = new Saver(this);
	this->graphics = NULL;
	this->current_screen = NULL;
	this->width = width;
	this->debuger = NULL;
	float aspect = (float)launcher->GetTargetHeight() / (float)launcher->GetTargetWidth();
	height = width * aspect;
	scale_factor = (float)launcher->GetTargetWidth() / width;
	launcher->LogIt("Game initialized");
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

void Game::SetScreen(Screen* screen){
	Debuger::StartCheckTime();
	delete current_screen;
	current_screen = screen;
	current_screen->Init();
	current_screen->Start();
    render_time = high_resolution_clock::now();
	Debuger::StopCheckTime("Screen loaded: ");
}

Screen* Game::GetCurrentScreen(){
	return current_screen;
}

void Game::Start(){
	try{
#ifdef CROSSDEBUG
		debuger = new Debuger(this);
#endif
		SetScreen(GetStartScreen());
		launcher->LogIt("Start screen load successfully");
	}catch(string& msg){
		msg = "Exception: " + msg;
		launcher->LogIt(msg);
#ifdef WIN
		LauncherWIN* win = (LauncherWIN*)launcher;
		win->ShowMessage(msg);
#endif 
		Exit();
	}
}

void Game::Update(){
	try{
		time_point<high_resolution_clock> now = high_resolution_clock::now();
		long long rend = duration_cast<microseconds>(now - render_time).count();
		render_time = high_resolution_clock::now();

		//GetCurrentScreen()->Update((float)(rend / 1000000.));
		GetCurrentScreen()->Update(0.05f);
		now = high_resolution_clock::now();
		long long up = duration_cast<microseconds>(now - render_time).count();
		float milis = up / 1000.f;
		if(milis < 10){
			launcher->Sleep(10 - milis);
		}
#ifdef CROSSDEBUG
		debuger->Display((float)rend);
		debuger->SetUpdateTime((float)up);
#endif
	}catch(string msg){
		msg = "Exception: " + msg;
		launcher->LogIt(msg);
#ifdef WIN
		LauncherWIN* win = (LauncherWIN*)launcher;
		win->ShowMessage(msg);
#endif 
		Exit();
	}
}

void Game::Exit(){
	exit(0);
}

Game::~Game(){ }
