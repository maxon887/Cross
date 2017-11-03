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
#include "Texture.h"
#include "Graphics3D/Simple/TriangleScene.h"
#include "AudioScreen.h"

#include "Libs/ImGui/imgui.h"

void MainScreen::Start(){
	Screen::Start();
	MenuBar::Init();
	SetName("Main");
	SetBackground(Color(0.3f));
}

void MainScreen::Stop(){
	MenuBar::Release();
	Screen::Stop();
}

void MainScreen::Update(float sec){
	Screen::Update(sec);
	MenuBar::Update(sec);
	MenuBar::ShowMenu();

	ImGui::PushFont(font_big);

	if(!system->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2(GetWidth() / 3.f, GetHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(GetWidth() / 2.f, GetHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Demo");
	} else {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight() - menu_height));
		ImGui::SetNextWindowPos(ImVec2(0, menu_height));
		ImGui::Begin("Demo", 0, ImGuiWindowFlags_NoCollapse |
								ImGuiWindowFlags_NoMove |
								ImGuiWindowFlags_NoTitleBar |
								ImGuiWindowFlags_NoResize |
								ImGuiWindowFlags_NoBringToFrontOnFocus);
	}
	if(ImGui::CollapsingHeader("Graphics", ImGuiTreeNodeFlags_DefaultOpen)) {
		if(ImGui::TreeNode("Simple")) {
			if(ImGui::MenuButton("Triangle")) {
				game->SetScreen(new TriangleScene());
			}
			if(ImGui::MenuButton("Solid Model")) {
			
			}
			if(ImGui::MenuButton("Textured Model")) {

			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Light")) {
			if(ImGui::MenuButton("Material")) {

			}
			if(ImGui::MenuButton("Directional Light")) {

			}
			if(ImGui::MenuButton("Point Light")) {

			}
			if(ImGui::MenuButton("Spot Light")) {

			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Maps")) {
			if(ImGui::MenuButton("Naked")) {

			}
			if(ImGui::MenuButton("Diffuse")) {

			}
			if(ImGui::MenuButton("Specular")) {

			}
			if(ImGui::MenuButton("Roughness")) {

			}
			if(ImGui::MenuButton("Normal")) {

			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Misc")) {
			if(ImGui::MenuButton("Depth Test")) {

			}
			if(ImGui::MenuButton("Transparency")) {

			}
			if(ImGui::MenuButton("Skybox")) {

			}
			if(ImGui::MenuButton("Apocalypse Scene")) {

			}
			ImGui::TreePop();
		}
	}
	if(ImGui::Button("Audio", ImVec2(-1, 0))) {
		game->SetScreen(new AudioScreen());
	}
	if(ImGui::Button("GUI", ImVec2(-1, 0))) {
		game->SetScreen(new UIScreen());
	}
	ImGui::End();
	ImGui::PopFont();
	if(system->IsMobile()) {
		ImGui::PopStyleVar();
	}
}
