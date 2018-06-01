/*	Copyright © 2018 Maksim Lukyanov

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
#include "System.h"
#include "Internals/GraphicsGL.h"
#include "Internals/Audio.h"
#include "Input.h"
#include "Config.h"
#include "Scene.h"
#include "Utils/Debugger.h"
#include "ComponentFactory.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

using namespace cross;

Game*		cross::game		= nullptr;
System*		cross::system	= nullptr;
Audio*		cross::audio	= nullptr;
GraphicsGL* cross::gfxGL	= nullptr;
Input*		cross::input	= nullptr;
Config*		cross::config	= nullptr;

Game::Game() {
	system->LogIt("Game::Game()");
	input = new Input();
	config = new Config();
	component_factory = new ComponentFactory();
	component_factory->Register<Transform>("Transform");
	component_factory->Register<Mesh>("Mesh");
	component_factory->Register<Camera>("Camera");
}

Game::~Game() {
	system->LogIt("Game::~Game");
	delete component_factory;
	delete config;
	delete input;
	delete current_screen;
}

void Game::SetScreen(Screen* screen) {
	next_screen = screen;
	if(!current_screen) {	//in this case we need momently load new screen
		LoadNextScreen();
	}
}

Screen* Game::GetCurrentScreen() {
	return current_screen;
}

Scene* Game::GetCurrentScene() {
	return dynamic_cast<Scene*>(game->GetCurrentScreen());
}

ComponentFactory* Game::GetComponentFactory() {
	return component_factory;
}

void Game::Suspend() {
	system->LogIt("Game::Suspend");
	suspended = true;

	if(audio) {
		audio->Suspend();
	}

	if(config) {
		config->SaveGameConfig();
		config->SaveUserConfig();
	}

	if(current_screen != nullptr) {
		current_screen->Suspend();
	}
}

void Game::Resume() {
	system->LogIt("Game::Resume");
	suspended = false;
	if(audio) {
		audio->Resume();
	}
	timestamp = system->GetTime();
	if(current_screen) {
		current_screen->Resume();
	}
}

float Game::GetRunTime() const {
	return (float)(run_time / 1000000.f);
}

void Game::EngineUpdate() {
	if(!suspended) {
		SAFE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		U64 now = system->GetTime();
		U64 updateTime = now - timestamp;
		float secTime = (float)(updateTime / 1000000.);
		timestamp = now;
		run_time += updateTime;

		if(next_screen) {
			LoadNextScreen();
		}

		input->Update();
		game->PreUpdate(secTime);
		if(game->GetCurrentScreen()) {
			game->GetCurrentScreen()->PreUpdate(secTime);
			game->GetCurrentScreen()->Update(secTime);
			game->GetCurrentScreen()->PostUpdate(secTime);
		}
		game->Update(secTime);

		Debugger::Instance()->Update((float)updateTime);
		U64 cpuTime = system->GetTime() - timestamp;
		Debugger::Instance()->SetCPUTime((float)cpuTime);

		float milis = cpuTime / 1000.f;
		if(milis < 5) {
			system->Sleep(5 - milis);
		}
	}
}

bool Game::IsSuspended() const {
	return suspended;
}

void Game::LoadNextScreen() {
	system->LogIt("Game::LoadNextScreen()");
	Debugger::Instance()->SetTimeCheck();

	if(current_screen) {
		current_screen->Stop();
		delete current_screen;
		current_screen = nullptr;
	}

	current_screen = next_screen;
	next_screen = nullptr;
	current_screen->Start();

	timestamp = system->GetTime();
	float loadTime = Debugger::Instance()->GetTimeCheck();
	system->LogIt("Screen(#) loaded in #ms", current_screen == nullptr? "" : current_screen->GetName(), String(loadTime, "%0.1f", 10));
	ScreenChanged.Emit(current_screen);
}
