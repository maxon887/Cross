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
#include "Screen.h"
#include "Camera2D.h"
#include "System.h"
#include "GraphicsGL.h"
#include "Game.h"
#include "Shaders/Shader.h"
#include "File.h"
#include "Texture.h"
#include "Config.h"

using namespace cross;

void Screen::Start() {
	camera2D = new Camera2D();
}

void Screen::Stop() {
	delete camera2D;
	for(pair<S32, Shader*> pair : shaders) {
		delete pair.second;
	}
	shaders.clear();
}

void Screen::Update(float sec) {
}

void Screen::PostUpdate(float sec) {
	camera2D->Update(sec);
}

float Screen::GetWidth() {
	return camera2D->GetViewWidth();
}

float Screen::GetHeight() {
	return camera2D->GetViewHeight();
}

const string& Screen::GetName() const {
	return name;
}

void Screen::SetName(const string& name) {
	this->name = name;
}

Camera2D* Screen::GetCamera() {
	return camera2D;
}

bool Screen::IsScene() const {
	return is_scene;
}

float Screen::GetScaleFactor() {
	return (float)system->GetWindowWidth() / GetWidth();
}

void Screen::SetBackground(const Color& c) {
	glClearColor(c.R, c.G, c.B, 1.f);
}

void Screen::EnableInputs(bool enable) {
	enable_inputs = enable;
}

Shader* Screen::GetShader(const string& shaderfile) {
	S32 hash = std::hash<string>{}(shaderfile);
	auto shaderIt = shaders.find(hash);
	if(shaderIt != shaders.end()) {
		return (*shaderIt).second;
	} else {
		Shader* shader = new Shader();
		shader->Load(shaderfile);
		shader->Compile();
		shaders[hash] = shader;
		return shader;
	}
}