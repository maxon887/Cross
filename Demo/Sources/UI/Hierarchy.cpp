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
#include "Hierarchy.h"
#include "Entity.h"
#include "Game.h"
#include "Scene.h"

#include "ThirdParty/ImGui/imgui.h"

void Hierarchy::Show(bool* open) {
	ImGui::Begin("Hierarchy", open);
	if(game->GetCurrentScreen()->IsScene()) {
		for(Entity* child : game->GetCurrentScene()->GetRoot()->GetChildren()) {
			BuildNode(child);
		}
	}
	ImGui::End();
}

void Hierarchy::BuildNode(Entity* entity) {
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

	bool open = ImGui::TreeNodeEx(entity->GetName().c_str(), flags);
	if(ImGui::IsItemClicked()) {
		selected_entity = entity;
	}
	if(open && !isLeaf) {
		for(Entity* child : entity->GetChildren()) {
			BuildNode(child);
		}
		ImGui::TreePop();
	}
}