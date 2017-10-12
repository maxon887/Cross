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

#include "Libs/ImGui/imgui.h"

void MainScreen::Start(){
	Screen::Start();
	SetBackground(Color(0.3f));
	down_del = input->ActionDown.Connect(this, &MainScreen::ActionDownHandle);
	move_del = input->ActionMove.Connect(this, &MainScreen::ActionMoveHandle);
	up_del = input->ActionUp.Connect(this, &MainScreen::ActionUpHandle);
}

void MainScreen::Stop(){
	Screen::Stop();
}

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void MainScreen::Update(float sec){
	Screen::Update(sec);

	ImGui::Begin("Main Window");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(GetWidth(), GetHeight()));
	ImGui::SetCursorPos(ImVec2(GetWidth() / 2.f, GetHeight() / 2.f));
	ImGui::Button("GUI");
	ImGui::End();

	/*
	{
		static float f = 0.0f;
		ImGui::Text("Hello, world!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		if(ImGui::Button("Test Window")) show_test_window ^= 1;
		if(ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// 2. Show another simple window, this time using an explicit Begin/End pair
	if(show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello from another window!");
		ImGui::End();
	}

	// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
	*/
}

void MainScreen::ActionDownHandle(Input::Action action) {
	action_down = true;
	action_pos = action.pos;
}

void MainScreen::ActionMoveHandle(Input::Action action) {
	action_pos = action.pos;
}

void MainScreen::ActionUpHandle(Input::Action action) {
	action_down = false;
	action_pos = action.pos;
}