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
#include "MenuBar.h"
#include "System.h"
#include "Demo.h"
#include "Transform.h"
#include "UI/CameraController.h"
#include "UI/SceneView.h"
#include "UI/FilesView.h"
#include "UI/ComponentsView.h"
#include "UI/Log.h"
#include "UI/Stats.h"
#include "UI/About.h"
#include "UI/LaunchView.h"
#include "UI/PropertiesView.h"
#include "Scenes/DemoScene.h"
#include "File.h"

#include "ThirdParty/ImGui/imgui.h"

MenuBar::MenuBar() {
	CameraController* cameraController = new CameraController();
	FilesView* filesView = new FilesView();
	SceneView* sceneView = new SceneView();
	ComponentsView* componentsView = new ComponentsView(sceneView);
	PropertiesView* propertiesView = new PropertiesView(filesView);
	views.Add(filesView);
	views.Add(sceneView);
	views.Add(componentsView);
	views.Add(propertiesView);
	views.Add(cameraController);
	for(View* v : views) {
		if(v->IsVisible()) {
			v->Shown();
		}
	}

	log = new Log();
	stats = new Stats();
	about = new About();
}

MenuBar::~MenuBar() {
	for(View* v : views) {
		delete v;
	}
	views.Clear();

	delete about;
	delete stats;
	delete log;
}

void MenuBar::Update(float sec) {
	for(View* v : views) {
		v->Run(sec);
	}
	if(show_style_editor) {
		ImGui::Begin("Style Editor", &show_style_editor, ImVec2(ImGui::GetWindowWidth() / 2.f, ImGui::GetWindowHeight() / 2.f));
		ImGui::ShowStyleEditor();
		ImGui::End();
	}

	log->Run(sec);
	stats->Run(sec);
	about->Run(sec);
}

void MenuBar::ShowMenu() {
	if(system->IsMobile()) {
		ImGui::PushFont(demo->big_font);
	}

	if(ImGui::BeginMainMenuBar()) {
		bool mainScreen = false;
		if(game->GetCurrentScreen()->GetName() == "Main") {
			mainScreen = true;
		}

		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("New Scene")) {
				Scene* scene = new DemoScene();
				game->SetScreen(scene);
			}

			if(ImGui::MenuItem("Open Scene")) {
				String sceneFile = system->OpenFileDialog("*.scn");
				if(sceneFile != "") {
					Scene* scene = new DemoScene();
					if(!scene->Load(sceneFile)) {
						delete scene;
						CROSS_ASSERT(false, "Can not load scene file, sorry");
					} else {
						game->SetScreen(scene);
					}
				}
			}

			if(ImGui::MenuItem("Save Scene", 0, false, game->GetCurrentScene() != nullptr)) {
				String filename = system->OpenFileDialog("*.scn", true);
				if(filename != "") {
					String extencion = File::ExtensionFromFile(filename);
					if(extencion.IsEmpty()) {
						filename += ".scn";
					}
					game->GetCurrentScene()->Save(filename);
				}
			}

			if(ImGui::MenuItem("Back to Main", 0, false, !demo->GetLaunchView()->IsVisible())) {
				demo->ToMain();
			}


			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("View")) {
			for(View* v : views) {
				if(v->VisibleInMenu()) {
					bool selected = v->IsVisible();
					bool available = v->AvailableInMenu();
					if(ImGui::MenuItem(v->GetName(), "", &selected, available)) {
						if(selected) {
							v->Show();
						} else {
							v->Hide();
						}
					}
				}
			}
			ImGui::EndMenu();
		}

		ImVec2 helpSize = ImGui::CalcTextSize("Help");
		ImGui::SameLine(ImGui::GetWindowWidth() - helpSize.x * 2.f);
		if(ImGui::BeginMenu("Help")) {
			if(ImGui::MenuItem("Log")) {
				log->Show();
			}
			if(ImGui::MenuItem("Stats")) {
				stats->Show();
			}
			if(ImGui::MenuItem("Style Editor")) {
				show_style_editor = true;
			}
			if(ImGui::MenuItem("About")) {
				about->Show();
			}
			ImGui::EndMenu();
		}
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 size = ImGui::GetItemRectSize();
		menu_height = size.y +style.FramePadding.y * 2.f - style.ItemSpacing.y * 2.f;// +style.ItemSpacing.y;
		ImGui::EndMainMenuBar();
	}

	if(system->IsMobile()) {
		ImGui::PopFont();
	}
}

void MenuBar::CloseAllViews() {
	for(View* v : views) {
		v->Hide();
	}
}

float MenuBar::GetHeight() const {
	return menu_height;
}
