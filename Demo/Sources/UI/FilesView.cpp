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
		bool open = ImGui::TreeNodeEx(child.name, node_flags);
		if(open) {
			if(!child.initialized) {
				InitNode(child);
			}
			BuildNote(child);
			ImGui::TreePop();
		}
	}
	for(const String& file : node.files) {
		ImGui::TreeNodeEx(file, leaf_flags);
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			String filename = node.path + node.name + "/" + file;
			system->OpenFileExternal(filename);
		}
	}
}
