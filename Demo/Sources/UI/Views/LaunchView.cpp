#include "LaunchView.h"
#include "Demo.h"
#include "System.h"
#include "ImGuiScreen.h"
#include "Texture.h"
#include "UI/MenuBar.h"
#include "Scenes/Simple/TriangleScene.h"
#include "Scenes/Light/DirectionalLightScene.h"
#include "Scenes/Light/PointLightScene.h"
#include "Scenes/Light/SpotLightScene.h"
#include "Scenes/Light/MultiLightScene.h"
#include "Scenes/Maps/NakedScene.h"
#include "Scenes/Maps/DiffuseScene.h"
#include "Scenes/Maps/SpecularScene.h"
#include "Scenes/Maps/RoughnessScene.h"
#include "Scenes/Maps/NormalScene.h"
#include "Scenes/Misc/DepthScene.h"
#include "Scenes/Misc/TransparencyScene.h"
#include "Scenes/Misc/SkyboxScene.h"
#include "Tests/StringTest.h"
#include "Tests/ArrayTest.h"
#include "Tests/AutoTests.h"
#include "AudioScreen.h"
#include "Shooter/Shooter.h"

#include "ThirdParty/ImGui/imgui.h"

LaunchView::LaunchView() : View("Demos")
{ }

void LaunchView::PreUpdate() {
	ImGui::PushFont(demo->big_font);

	if(!os->IsMobile()) {
		ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth() / 3.f, (float)os->GetWindowHeight() / 3.f * 2.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2((float)os->GetWindowWidth() / 2.f, (float)os->GetWindowHeight() / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
	} else {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth(), ((float)os->GetWindowHeight() - demo->GetMenuBar()->GetHeight())));
		ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
		SetFlags(	ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoBringToFrontOnFocus);
	}
}

void LaunchView::Update(float sec) {
	if(ImGui::CollapsingHeader("Scenes", ImGuiTreeNodeFlags_DefaultOpen)) {
		if(ImGui::MenuButton("Shooter Example")) {
			const String filename = "Shooter/Shooter.scn";
			Shooter* shooter = CREATE Shooter();
			CROSS_ASSERT(shooter->Load(filename), "Can not load scene(#)", filename);
			game->SetScreen(shooter);
		}
		if(ImGui::MenuButton("Apocalypse Scene")) {
			const String filename = "Scenes/ApocalypticCity/ApocalypticCity.scn";
			CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
		}
		if(ImGui::TreeNode("Simple")) {
			if(ImGui::MenuButton("Triangle")) {
				game->SetScreen(CREATE TriangleScene());
			}
			if(ImGui::MenuButton("Solid Model")) {
				const String filename = "Scenes/Cube.scn";
				CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
			}
			if(ImGui::MenuButton("Textured Model")) {
				const String filename = "Scenes/TexturedCube.scn";
				CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Light")) {
			if(ImGui::MenuButton("Material")) {
				const String filename = "Scenes/Material.scn";
				CROSS_ASSERT(demo->LoadScene(filename), "Can not load scene(#)", filename);
			}
			if(ImGui::MenuButton("Directional Light")) {
				game->SetScreen(CREATE DirectionalLightScene());
			}
			if(ImGui::MenuButton("Point Light")) {
				game->SetScreen(CREATE PointLightScene());
			}
			if(ImGui::MenuButton("Spot Light")) {
				game->SetScreen(CREATE SpotLightScene());
			}
			if(ImGui::MenuButton("Multi-Light")) {
				game->SetScreen(CREATE MultiLightScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Maps")) {
			if(ImGui::MenuButton("Naked")) {
				game->SetScreen(CREATE NakedScene());
			}
			if(ImGui::MenuButton("Diffuse")) {
				game->SetScreen(CREATE DiffuseScene());
			}
			if(ImGui::MenuButton("Specular")) {
				game->SetScreen(CREATE SpecularScene());
			}
			if(ImGui::MenuButton("Roughness")) {
				game->SetScreen(CREATE RoughnessScene());
			}
			if(ImGui::MenuButton("Normal")) {
				game->SetScreen(CREATE NormalScene());
			}
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Misc")) {
			if(ImGui::MenuButton("Depth Test")) {
				game->SetScreen(CREATE DepthScene());
			}
			if(ImGui::MenuButton("Transparency")) {
				game->SetScreen(CREATE TransparencyScene());
			}
			if(ImGui::MenuButton("Skybox")) {
				game->SetScreen(CREATE SkyboxScene());
			}
			ImGui::TreePop();
		}
	}

	if(ImGui::CollapsingHeader("Tests")) {
		ImVec2 cursorPos = ImGui::GetCursorPos();
		cursorPos.x += ImGui::GetStyle().IndentSpacing;
		ImGui::SetCursorPos(cursorPos);
		if(ImGui::MenuButton("String Test")) {
			game->SetScreen(CREATE StringTest());
		}
		cursorPos = ImGui::GetCursorPos();
		cursorPos.x += ImGui::GetStyle().IndentSpacing;
		ImGui::SetCursorPos(cursorPos);
		if(ImGui::MenuButton("Array Test")) {
			game->SetScreen(CREATE ArrayTest());
		}
		cursorPos = ImGui::GetCursorPos();
		cursorPos.x += ImGui::GetStyle().IndentSpacing;
		ImGui::SetCursorPos(cursorPos);
		if(ImGui::MenuButton("Auto Tests")) {
			demo->GetAutoTests()->StartTests();
		}
	}
	if(ImGui::Button("Audio", ImVec2(-1, 0))) {
		game->SetScreen(CREATE AudioScreen());
	}
	if(ImGui::Button("GUI", ImVec2(-1, 0))) {
		game->SetScreen(CREATE ImGuiScreen());
	}
}

void LaunchView::PostUpdate() {
	if(os->IsMobile()) {
		ImGui::PopStyleVar();
	}
	ImGui::PopFont();
}