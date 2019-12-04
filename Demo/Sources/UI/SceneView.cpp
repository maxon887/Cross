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
#include "SceneView.h"
#include "Entity.h"
#include "Demo.h"
#include "Scene.h"
#include "MenuBar.h"
#include "System.h"
#include "Camera.h"
#include "Transform.h"
#include "Utils/FreeCameraScene.h"

#include "ThirdParty/ImGui/imgui.h"

SceneView::SceneView() : View("Scene")
{
	game->ScreenChanged.Connect(this, &SceneView::OnSceneChanged);
}

void SceneView::PreUpdate() {
	if(system->IsMobile()) {
		if(system->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
			ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth() / 3.f,
				(float)(system->GetWindowHeight() - demo->GetMenuBar()->GetHeight())),
				ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()), ImGuiCond_FirstUseEver);
		} else {
			ImGui::PushFont(demo->big_font);
			ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth(), (float)(system->GetWindowHeight() - demo->GetMenuBar()->GetHeight())));
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()));
		}
	}
}

void SceneView::Update(float sec) {
	if(game->GetCurrentScene()) {
		for(Entity* child : game->GetCurrentScene()->GetRoot()->GetChildren()) {
			BuildNode(child);
		}
	} else {
		selected_entity = nullptr;
	}

	if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
		selected_entity = nullptr;
		editing = false;
		EntitySelected.Emit(selected_entity);
	}

	ContextMenu();
}

void SceneView::PostUpdate() {
	if(system->IsMobile()) {
		if(system->GetDeviceOrientation() != System::Orientation::LANDSCAPE) {
			ImGui::PopFont();
		}
	}
}

Entity* SceneView::GetSelectedEntity() {
	return selected_entity;
}

void SceneView::OnSceneChanged(Screen*) {
	selected_entity = nullptr;
}

void SceneView::LookAtObject() {
	FreeCameraScene* scene = static_cast<FreeCameraScene*>(game->GetCurrentScene());
	if(!selected_entity->HasComponent<Camera>() && selected_entity->HasComponent<Transform>()) {
		scene->LookAtTarget(selected_entity->GetTransform()->GetPosition());
	}
}

void SceneView::BuildNode(Entity* entity) {

	if(system->IsMobile() && system->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
		ImGui::PushFont(demo->normal_font);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, SCALED(10.f));
	}

	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	ImGuiTreeNodeFlags selected = entity == selected_entity ? ImGuiTreeNodeFlags_Selected : 0;

	ImGuiTreeNodeFlags flags = 0;
	bool isLeaf = entity->GetChildren().size() == 0;
	if(!isLeaf) {
		flags = node_flags | selected;
	} else {
		flags = leaf_flags | selected;
	}

	bool open = false;
	if(editing && entity == selected_entity) {
		ImVec2 cursorPos = ImGui::GetCursorPos();
		cursorPos.x += ImGui::GetStyle().IndentSpacing;
		ImGui::SetCursorPos(cursorPos);

		char buffer[256];
		strcpy(buffer, entity->GetName().ToCStr());
		ImGui::InputText("", buffer, 256);

		if(ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			editing = false;
		}

		if(ImGui::IsKeyPressed(ImGuiKey_Enter)) {
			entity->SetName(buffer);
			editing = false;
		}

		if(!ImGui::IsItemActive() && !clicked) {
			editing = false;
		}

		if(clicked) {
			ImGui::SetKeyboardFocusHere();
			clicked = false;
		}

	} else {
		open = ImGui::TreeNodeEx(entity->GetName(), flags);
	}

	if(ImGui::IsItemClicked()) {
		clicked = true;
		if(selected_entity == entity) {
			editing = true;
		} else {
			selected_entity = entity;
			editing = false;
			EntitySelected.Emit(entity);
			LookAtObject();
		}
	}

	if(open && !isLeaf) {
		for(Entity* child : entity->GetChildren()) {
			BuildNode(child);
		}
		ImGui::TreePop();
	}

	if(system->IsMobile() && system->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
		ImGui::PopFont();
		ImGui::PopStyleVar(2);
	}
}

void SceneView::ContextMenu() {
	if(ImGui::BeginPopupContextWindow("SceneOptions")) {
		bool enabled = game->GetCurrentScene() != nullptr ? true : false;
		if(ImGui::MenuItem("New Entity", nullptr, false, enabled)) {
			Entity* newEntity = new Entity("NewEntity");
			game->GetCurrentScene()->AddEntity(newEntity);
		}
		if(ImGui::MenuItem("Import Model", nullptr, false, enabled)) {
			String filename = system->OpenFileDialog();
			Model* model = game->GetCurrentScene()->GetModel(filename);
			Entity* entity = model->GetHierarchy();
			game->GetCurrentScene()->AddEntity(entity);
		}

		ImGui::EndPopup();
	}
}
