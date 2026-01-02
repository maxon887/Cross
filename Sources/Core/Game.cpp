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
#include "Graphics.h"
#include "Internals/GraphicsGL.h"
#include "Internals/Audio.h"
#include "Input.h"
#include "Config.h"
#include "Scene.h"
#include "Utils/Debugger.h"
#include "Factory.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"

using namespace cross;

Game*		cross::game		= nullptr;
System*		cross::os		= nullptr;
Graphics*	cross::gfx		= nullptr;
Audio*		cross::audio	= nullptr;
GraphicsGL* cross::gfxGL	= nullptr;
Input*		cross::input	= nullptr;
Config*		cross::config	= nullptr;

Game::Game() {
	os->LogIt("Game::Game()");
	gfx = CREATE Graphics();
	input = CREATE Input();
	config = CREATE Config();
	component_factory = CREATE Factory<Component>();
	component_factory->Register<Transform>("Transform");
	component_factory->Register<Mesh>("Mesh");
	component_factory->Register<Camera>("Camera");
	component_factory->Register<Light>("Light");
}

Game::~Game() {
	os->LogIt("Game::~Game");
	delete component_factory;
	delete config;
	delete input;
	delete current_screen;
	delete gfx;
}

void Game::Start() {
	gfx->Start();
}

void Game::Stop() {
	gfx->Stop();
}

void Game::Update(float sec) {
	gfx->Update();
}

void Game::SetScreen(Screen* screen) {
	next_screen = screen;
	if(!current_screen) {	//in this case we need instantly load new screen
		LoadNextScreen();
	}
}

Screen* Game::GetCurrentScreen() const {
	return current_screen;
}

Scene* Game::GetCurrentScene() const {
	return dynamic_cast<Scene*>(GetCurrentScreen());
}

Factory<Component>* Game::GetComponentFactory() const {
	return component_factory;
}

void Game::Suspend() {
	os->LogIt("Game::Suspend");
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
	os->LogIt("Game::Resume");
	suspended = false;
	if(audio) {
		audio->Resume();
	}
	timestamp = os->GetTime();
	if(current_screen) {
		current_screen->Resume();
	}
}

float Game::GetRunTime() const {
	return (float)run_time / 1000000.f;
}

void Game::EngineUpdate() {
	if(!suspended) {
		SAFE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		U64 now = os->GetTime();
		U64 updateTime = now - timestamp;
		float secTime = (float)updateTime / 1000000.f;
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
		game->PostUpdate(secTime);

		Debugger::Instance()->Update((float)updateTime);
		U64 cpuTime = os->GetTime() - timestamp;
		Debugger::Instance()->SetCPUTime((float)cpuTime);

		float milis = (float)updateTime / 1000.f;
		if(milis < 5.f) {
			os->Sleep(5.f - milis);
		}
	}
}

bool Game::IsSuspended() const {
	return suspended;
}

void Game::LoadNextScreen() {
	os->LogIt("Game::LoadNextScreen()");
	Debugger::Instance()->SetTimeCheck();

	if(current_screen) {
		current_screen->Stop();
		delete current_screen;
	}

	current_screen = next_screen;
	next_screen = nullptr;

	timestamp = os->GetTime();
	ScreenChanged.Emit(current_screen);
	current_screen->Start();

	float loadTime = Debugger::Instance()->GetTimeCheck();
	os->LogIt("Screen(#) loaded in #ms", current_screen == nullptr ? "" : current_screen->GetName(), String(loadTime, "%0.1f", 10));
}
