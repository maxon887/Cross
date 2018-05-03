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

void FilesView::Content(float sec) {
	Array<String> folders = system->GetSubDirectories(system->AssetsPath());
	for(const String& name : folders) {
		BuildNote(name, true);
	}
	Array<String> files = system->GetFilesInDirectory(system->AssetsPath());
	for(const String& name : files) {
		BuildNote(name, false);
	}
}

void FilesView::BuildNote(const String &name, bool isFolder) {
	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	if(!isFolder) {
		ImGui::TreeNodeEx(name, leaf_flags);
	} else {
		bool open = ImGui::TreeNodeEx(name, node_flags);
		if(open) {
			Array<String> folders = system->GetSubDirectories(system->AssetsPath() + "/" + name);
			for(const String& name : folders) {
				BuildNote(name, true);
			}
			Array<String> files = system->GetFilesInDirectory(system->AssetsPath() + "/" + name);
			for(const String& name : files) {
				BuildNote(name, false);
			}
			ImGui::TreePop();
		}
	}
}
