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
#include "Utils/Debuger.h"
#ifdef WIN
#include "Platform/Windows/LauncherWIN.h"
#endif 

#include <stdlib.h>

using namespace cross;
using namespace chrono;

mutex cross::global_mutex;

Launcher*	cross::launcher = NULL;
Graphics*	cross::graphics = NULL;
Graphics3D* cross::gfx3D = NULL;
Input*		cross::input = NULL;
Config*		cross::config = NULL;

Game::Game(Launcher* launcher){
	Init(launcher);
	this->width = (float)launcher->GetTargetWidth();
	this->height = (float)launcher->GetTargetHeight();
	scale_factor = (float)launcher->GetTargetWidth() / width;
}

Game::Game(Launcher* launcher, float width){
	Init(launcher);
	this->width = width;
	float aspect = (float)launcher->GetTargetHeight() / (float)launcher->GetTargetWidth();
	height = width * aspect;
    scale_factor = (float)launcher->GetTargetWidth() / width;
}

Game::Game(Launcher* launcher, float width, float height){
	Init(launcher);
	this->width = width;
	this->height = height;
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

void Game::SetScreen(Screen* screen){
	launcher->LogIt("Game->SetScreen()");
	Debuger::StartCheckTime();
	delete current_screen;
	current_screen = screen;
	current_screen->Start();
    render_time = high_resolution_clock::now();
	Debuger::StopCheckTime("Screen loaded: ");
}

Screen* Game::GetCurrentScreen(){
	return current_screen;
}

void Game::Start(){
	launcher->LogIt("Game->Start()");
	try{
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

void Game::Suspend(){
	current_screen->Suspend();
}

void Game::Resume(){
	render_time = high_resolution_clock::now();
}

void Game::Update(){
	try{
		time_point<high_resolution_clock> now = high_resolution_clock::now();
		long long rend = duration_cast<microseconds>(now - render_time).count();
		render_time = high_resolution_clock::now();
		GetCurrentScreen()->Update((float)(rend / 1000000.));
		now = high_resolution_clock::now();
		long long up = duration_cast<microseconds>(now - render_time).count();
		float milis = up / 1000.f;
		if(milis < 5){
			launcher->Sleep(5 - milis);
		}
		debuger->Display((float)rend);
		debuger->SetUpdateTime((float)up);
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

bool Game::Is3D(){
#ifdef C3D
	return true;
#else
	return false;
#endif
}

void Game::Exit(){
	exit(0);
}
void Game::Init(Launcher* launcher){
	launcher = launcher;
	input = new Input();
	config = new Config(launcher->DataPath());
	debuger = new Debuger(this);
	graphics = NULL;
	gfx3D = NULL;
	this->current_screen = NULL;
}

Game::~Game(){
	launcher->LogIt("Game destructor");
	delete current_screen;
	delete input;
	delete config;
	delete debuger;
	launcher->LogIt("Destructor finished");
}
