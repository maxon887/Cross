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
#include "Scene.h"
#include "Input.h"
#include "Config.h"
#include "Utils/Debugger.h"
#include "Audio.h"
#include "Graphics2D.h"
#include "Camera2D.h"
#ifdef WIN
#include "Platform/Windows/LauncherWIN.h"
#undef GetMessage
#endif 

#include <stdlib.h>

using namespace cross;
using namespace chrono;

Launcher*	cross::launcher = NULL;
GraphicsGL* cross::gfxGL = NULL;
Graphics2D* cross::gfx2D = NULL;
Graphics3D* cross::gfx3D = NULL;
Input*		cross::input = NULL;
Config*		cross::config = NULL;

Game::Game() :
	current_screen(nullptr),
	on_scene(false),
	run_time(0.f)
{
	launcher->LogIt("Game::Game()");
	input = new Input();
	config = new Config(launcher->DataPath());
}

Game::~Game(){
	launcher->LogIt("Game::~Game");
	delete current_screen;
	delete input;
	delete config;
}


void Game::SetScreen(Screen* screen){
	launcher->LogIt("Game::SetScreen()");
	on_scene = false;
	Debugger::Instance()->StartCheckTime();
	if(current_screen){
		current_screen->Stop();
		delete current_screen;
	}
	current_screen = screen;
	current_screen->Start();
    render_time = high_resolution_clock::now();
	Debugger::Instance()->StopCheckTime("Screen loaded: ");
}

void Game::SetScene(Scene* scene){
	SetScreen(scene);
	on_scene = true;
}

Screen* Game::GetCurrentScreen(){
	return current_screen;
}

Scene* Game::GetCurrentScene(){
	if(on_scene){
		return (Scene*)current_screen;
	}else{
		throw CrossException("Current game state does not have 3D scene");
	}
}

void Game::Suspend(){
	if(current_screen != nullptr) {
		current_screen->Suspend();
	}
}

void Game::Resume(){
	render_time = high_resolution_clock::now();
	if(current_screen != nullptr) {
		current_screen->Resume();
	}
}

float Game::GetRunTime(){
	return run_time;
}

void Game::Update(){
	time_point<high_resolution_clock> now = high_resolution_clock::now();
	long long rend = duration_cast<microseconds>(now - render_time).count();
	render_time = high_resolution_clock::now();
	run_time += (float)(rend / 1000000.);
	gfx2D->Update();
	GetCurrentScreen()->Update((float)(rend / 1000000.));
	now = high_resolution_clock::now();
	long long up = duration_cast<microseconds>(now - render_time).count();
	float milis = up / 1000.f;
	if(milis < 5){
		launcher->Sleep(5 - milis);
	}
	Debugger::Instance()->Display((float)rend);
	Debugger::Instance()->SetUpdateTime((float)up);
}

void Game::Exit(){
#ifdef WIN
	exit(0);
#endif // WIN
}