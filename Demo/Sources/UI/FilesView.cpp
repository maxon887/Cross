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
#include "FilesView.h"
#include "System.h"
#include "File.h"
#include "Game.h"
#include "Scenes/DemoScene.h"

#include <algorithm>

#include "ThirdParty/ImGui/imgui.h"

FilesView::FilesView() : View("Files") { }

void FilesView::Shown() {
	if(!file_tree.initialized) {
		String assPath = system->AssetsPath();
		file_tree.path = assPath.SubString(0, assPath.Length() - 1);
		InitNode(file_tree);
	}
}

void FilesView::Content(float sec) {
	BuildNote(file_tree);
}

void FilesView::InitNode(Node& node) {
	String path = node.path + node.name + "/";
	Array<String> folders = system->GetSubDirectories(path);
	sort(folders.begin(), folders.end());
	for(const String& folder : folders) {
		Node newNode;
		newNode.path = path;
		newNode.name = folder;
		node.folders.push_back(newNode);
	}
	node.files = system->GetFilesInDirectory(path);
	node.initialized = true;
}

void FilesView::BuildNote(Node& node) {
	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	for(Node& child : node.folders) {
		String filepath = child.path + child.name;
		ImGuiTreeNodeFlags flags = filepath == selected_path ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags;

		bool open = ImGui::TreeNodeEx(child.name, flags);

		if(ImGui::IsItemClicked()) {
			selected_path = filepath;
		}

		if(open) {
			if(!child.initialized) {
				InitNode(child);
			}
			BuildNote(child);
			ImGui::TreePop();
		}
	}
	for(const String& file : node.files) {
		String filepath = node.path + node.name + "/" + file;
		ImGuiTreeNodeFlags flags = filepath == selected_path ? leaf_flags | ImGuiTreeNodeFlags_Selected : leaf_flags;
		ImGui::TreeNodeEx(file, flags);
		if(ImGui::IsItemClicked()) {
			selected_path = filepath;
			filepath.Remove(system->AssetsPath());
			FileSelected.Emit(filepath);
		}
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			FileDoubleClicked(filepath);
		}
	}
}

void FilesView::FileDoubleClicked(const String& filename) {
	String extencion = File::ExtensionFromFile(filename);
	if(extencion == "scn") {
		Scene* scene = new DemoScene();
		if(!scene->Load(filename, false)) {
			delete scene;
			CROSS_ASSERT(false, "Can not load scene file, sorry");
		} else {
			game->SetScreen(scene);
		}
	} else {
		system->OpenFileExternal(filename);
	}
}
