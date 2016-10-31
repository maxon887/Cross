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
#include "Input.h"
#include "Config.h"
#include "Scene.h"
#include "System/Debugger.h"
#include "Graphics2D.h"

using namespace cross;

Game*		cross::game		= NULL;
Launcher*	cross::launcher = NULL;
GraphicsGL* cross::gfxGL	= NULL;
Graphics2D* cross::gfx2D	= NULL;
Graphics3D* cross::gfx3D	= NULL;
Input*		cross::input	= NULL;
Config*		cross::config	= NULL;

Game::Game() :
	current_screen(NULL),
	next_screen(NULL),
	on_scene(false),
	next_is_scene(false),
	run_time(0),
	render_time(0)
{
	launcher->LogIt("Game::Game()");
	input = new Input();
	config = new Config();
}

Game::~Game(){
	launcher->LogIt("Game::~Game");
	delete current_screen;
	delete config;
	delete input;
}

Screen* Game::GetStartScreen(){
	return current_screen;
}

void Game::Start(){
	gfxGL->Start();
}

void Game::SetScreen(Screen* screen, bool scene){
	next_is_scene = scene;
	next_screen = screen;
	if(!current_screen){
		ReloadScreen();
	}
}

void Game::SetScene(Scene* scene){
	SetScreen(scene, true);
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
	render_time = launcher->GetTime();
	if(current_screen != nullptr) {
		current_screen->Resume();
	}
}

float Game::GetRunTime(){
	return (float)(run_time / 1000000.f);
}

void Game::Update(){
	U64 now = launcher->GetTime();
	U64 frame = now - render_time;
	render_time = now;
	run_time += frame;

	gfxGL->PreProcessFrame();
	gfx2D->Update((float)(frame / 1000000.));
	GetCurrentScreen()->Update((float)(frame / 1000000.));
	now = launcher->GetTime();
	gfxGL->PostProcessFrame();

	if(next_screen){
		ReloadScreen();
	}

	U64 update = now - render_time;
	float milis = update / 1000.f;
	if(milis < 5){
		launcher->Sleep(5 - milis);
	}
	Debugger::Instance()->Display((float)frame);
	Debugger::Instance()->SetUpdateTime((float)update);
}

void Game::Exit(){
#ifdef WIN
	exit(0);
#endif // WIN
}

void Game::ReloadScreen(){
	launcher->LogIt("Game::ReloadScreen()");
	Debugger::Instance()->StartCheckTime();

	if(current_screen){
		current_screen->Stop();
		delete current_screen;
	}
	current_screen = next_screen;
	next_screen = NULL;
	on_scene = next_is_scene;
	current_screen->Start();
	render_time = launcher->GetTime();
	Debugger::Instance()->StopCheckTime("Screen reloaded: ");
}