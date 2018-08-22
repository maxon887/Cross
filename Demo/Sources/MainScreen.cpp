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
#include "MainScreen.h"
#include "Demo.h"
#include "System.h"
#include "UIScreen.h"
#include "Texture.h"
#include "UI/MenuBar.h"
#include "Graphics3D/Simple/TriangleScene.h"
#include "Graphics3D/Simple/SolidModelScene.h"
#include "Graphics3D/Simple/TexturedModelScene.h"
#include "Graphics3D/Light/MaterialScene.h"
#include "Graphics3D/Light/DirectionalLightScene.h"
#include "Graphics3D/Light/PointLightScene.h"
#include "Graphics3D/Light/SpotLightScene.h"
#include "Graphics3D/Light/MultiLightScene.h"
#include "Graphics3D/Maps/NakedScene.h"
#include "Graphics3D/Maps/DiffuseScene.h"
#include "Graphics3D/Maps/SpecularScene.h"
#include "Graphics3D/Maps/RoughnessScene.h"
#include "Graphics3D/Maps/NormalScene.h"
#include "Graphics3D/Misc/DepthScene.h"
#include "Graphics3D/Misc/TransparencyScene.h"
#include "Graphics3D/Misc/SkyboxScene.h"
#include "Graphics3D/Misc/ApocalypseScene.h"
#include "Physics/Ocean.h"
#include "Physics/HardConstraints.h"
//#include "Physics/Bridge.h"
//#include "Physics/Suppressors.h"
#include "AudioScreen.h"

#include "ThirdParty/ImGui/imgui.h"

void MainScreen::Start() {
	Screen::Start();
	Screen::SetName("Main");
	Screen::SetBackground(Color(0.3f));
}

void MainScreen::Stop(){
	Screen::Stop();
}

void MainScreen::Update(float sec) {
	Screen::Update(sec);

	ImGui::PushFont(demo->big_font);

	if(!system->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2(system->GetWindowWidth() / 3.f, system->GetWindowHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() / 2.f, system->GetWindowHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Demo");
	} else {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth(), (float)(system->GetWindowHeight() - demo->GetMenuBar()->GetHeight())));
		ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
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
				game->SetScreen(new SolidModelScene());
			}
			if(ImGui::MenuButton("Textured Model")) {
				game->SetScreen(new TexturedModelScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Light")) {
			if(ImGui::MenuButton("Material")) {
				game->SetScreen(new MaterialScene());
			}
			if(ImGui::MenuButton("Directional Light")) {
				game->SetScreen(new DirectionalLightScene());
			}
			if(ImGui::MenuButton("Point Light")) {
				game->SetScreen(new PointLightScene());
			}
			if(ImGui::MenuButton("Spot Light")) {
				game->SetScreen(new SpotLightScene());
			}
			if(ImGui::MenuButton("Multi-Light")) {
				game->SetScreen(new MultiLightScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Maps")) {
			if(ImGui::MenuButton("Naked")) {
				game->SetScreen(new NakedScene());
			}
			if(ImGui::MenuButton("Diffuse")) {
				game->SetScreen(new DiffuseScene());
			}
			if(ImGui::MenuButton("Specular")) {
				game->SetScreen(new SpecularScene());
			}
			if(ImGui::MenuButton("Roughness")) {
				game->SetScreen(new RoughnessScene());
			}
			if(ImGui::MenuButton("Normal")) {
				game->SetScreen(new NormalScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Misc")) {
			if(ImGui::MenuButton("Depth Test")) {
				game->SetScreen(new DepthScene());
			}
			if(ImGui::MenuButton("Transparency")) {
				game->SetScreen(new TransparencyScene());
			}
			if(ImGui::MenuButton("Skybox")) {
				game->SetScreen(new SkyboxScene());
			}
			if(ImGui::MenuButton("Apocalypse Scene")) {
				game->SetScreen(new ApocalypseScene());
			}
			ImGui::TreePop();
		}
	}

	if(ImGui::CollapsingHeader("Physics")) {
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(SCALED(100.f), 30.f));

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::Indent(style.IndentSpacing);
		if(ImGui::MenuButton("Ocean")) {
			game->SetScreen(new Ocean());
		}
		if(ImGui::MenuButton("HardConstraints")) {
			game->SetScreen(new HardConstraints());
		}
		if(ImGui::MenuButton("Bridge")) {
			//game->SetScreen(new Bridge());
		}
		if(ImGui::MenuButton("Suppressors")) {
			//game->SetScreen(new Suppressors());
		}
		ImGui::Indent(-style.IndentSpacing);
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
