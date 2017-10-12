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
#include "MainScreen.h"
#include "Game.h"
#include "GraphicsGL.h"
#include "System.h"
#include "UIScreen.h"

#include "Libs/ImGui/imgui.h"

void MainScreen::Start(){
	Screen::Start();
	SetBackground(Color(0.3f));
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->addFont
}

void MainScreen::Stop(){
	Screen::Stop();
}

void MainScreen::Update(float sec){
	Screen::Update(sec);
	bool open;
	//ImGui::Begin("Main Window", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
	//	ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(GetWidth(), GetHeight()));
	//ImGui::SetCursorPos(ImVec2(GetWidth() / 2.f, GetHeight() / 2.f));
	ImGui::Begin("Demo");

	ImGui::Button("Graphics", ImVec2(ImGui::GetWindowWidth(), 0));

	if(ImGui::Button("GUI", ImVec2(ImGui::GetWindowWidth(), 0))) {
		game->SetScreen(new UIScreen());
	}
	ImGui::End();
}