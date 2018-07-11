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

void SceneView::WillContent() {
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

void SceneView::DidContent() {
	if(system->IsMobile()) {
		if(system->GetDeviceOrientation() != System::Orientation::LANDSCAPE) {
			ImGui::PopFont();
		}
	}
}

Entity* SceneView::GetSelectedEntity() {
	return selected_entity;
}

void SceneView::Content(float sec) {
	if(game->GetCurrentScene()) {
		for(Entity* child : game->GetCurrentScene()->GetRoot()->GetChildren()) {
			BuildNode(child);
		}
	} else {
		selected_entity = nullptr;
	}

	ContextMenu();
}

void SceneView::LookAtObject() {
	FreeCameraScene* scene = static_cast<FreeCameraScene*>(game->GetCurrentScene());
	if(!selected_entity->HasComponent<Camera>() && selected_entity->HasComponent<Transform>()) {
		scene->LookAtCamera(selected_entity->GetTransform()->GetPosition());
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

	bool open = ImGui::TreeNodeEx(entity->GetName(), flags);

	if(ImGui::IsItemClicked()) {
		selected_entity = entity;
		EntitySelected.Emit(entity);
		LookAtObject();
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

		ImGui::EndPopup();
	}
}
