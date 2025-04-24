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
#include "Graphics.h"
#include "File.h"
#include "Game.h"
#include "Demo.h"
#include "Material.h"
#include "Scenes/DemoScene.h"
#include "FileUtils.h"

#include <algorithm>

#include "ThirdParty/ImGui/imgui.h"

#undef CreateDirectory

using namespace std;

FilesView::FilesView() : View("Files", true) { }

void FilesView::Shown() {
	if(!file_tree.initialized) {
		InitNode(file_tree);
	}
	current_path = os->AssetsPath();
	game->ScreenChanged.Connect(this, &FilesView::OnScreenChanged);
}

void FilesView::Update(float sec) {
	BuildNote(file_tree);

	if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
		current_path = os->AssetsPath();
		FileSelected.Emit(current_path);
	}

	ContextMenu();
}

void FilesView::AskToShowFile(const String& filename) {
	String leftoverPath = filename;
	ForceOpenPath(leftoverPath, file_tree);
	current_path = filename;
}

void FilesView::InitNode(Node& node) {
	Array<String> folders = os->GetSubDirectories(os->AssetsPath() + node.path);
	sort(folders.begin(), folders.end());
	for(const String& folder : folders) {
		Node newNode;
		newNode.name = folder;
		newNode.path = node.path + folder + "/";
		node.folders.Add(newNode);
	}
	for(String& file : os->GetFilesInDirectory(os->AssetsPath() + node.path)) {
		node.files.Add(pair<String, String>(file, node.path + file));
	}
	node.initialized = true;
}

void FilesView::Refresh() {
	file_tree.files.Clear();
	file_tree.folders.Clear();
	InitNode(file_tree);
}

void FilesView::BuildNote(Node& node) {
	static const ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	static const ImGuiTreeNodeFlags leaf_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	//folders
	bool runOnce = true;
	for(Node& child : node.folders) {
		ImGuiTreeNodeFlags flags = node_flags;
		if(child.path == current_path) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		if(child.force_open) {
			ImGui::SetNextItemOpen(true);
			child.force_open = false;
			if(runOnce) {
				FileSelected.Emit(current_path);
				runOnce = false;
			}
		}

		bool open = ImGui::TreeNodeEx(child.name, flags);

		if((ImGui::IsMouseClicked(0) || ImGui::IsMouseClicked(1)) && ImGui::IsItemHovered()) {
			current_path = child.path;
		}

		if(open) {
			if(!child.initialized) {
				InitNode(child);
			}
			BuildNote(child);
			ImGui::TreePop();
		}
	}
	//files
	for(const pair<String, String>& file : node.files) {
		ImGuiTreeNodeFlags flags = file.second == current_path ? leaf_flags | ImGuiTreeNodeFlags_Selected : leaf_flags;
		ImGui::TreeNodeEx(file.first, flags);
		if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
			const String* filename = &file.second;
			ImGui::SetDragDropPayload("FilesViewDRAG", &filename, sizeof(String*));
			ImGui::TextUnformatted(file.first);
			ImGui::EndDragDropSource();
		}
		if((ImGui::IsMouseReleased(0) || ImGui::IsMouseClicked(1)) && ImGui::IsItemHovered()) {
			current_path = file.second;
			String filepath = current_path;
			FileSelected.Emit(filepath);
		}
		if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			String filepath = current_path;
			FileDoubleClicked(filepath);
		}
	}
}

void FilesView::FileDoubleClicked(const String& filename) {
	String extencion = File::ExtensionFromFile(filename);
	if(extencion == "scn") {
		Scene* scene = CREATE DemoScene();
		if(!scene->Load(filename)) {
			delete scene;
		} else {
			game->SetScreen(scene);
		}
	} else {
		os->OpenFileExternal(filename);
	}
}

void FilesView::ForceOpenPath(String& leftoverPath, Node& currentNode) {
	if(!currentNode.initialized) {
		InitNode(currentNode);
	}
	S32 foundSlash = leftoverPath.Find("/");
	if(foundSlash > 0) { //first character is slash. we found a folder. open it and go next
		String folderName = leftoverPath.SubString(0, foundSlash);
		for(auto& folder : currentNode.folders) {
			if(folder.name == folderName) {
				leftoverPath.Remove(0, foundSlash + 1);
				folder.force_open = true;
				ForceOpenPath(leftoverPath, folder);
			}
		}
	}
}

void FilesView::ContextMenu() {
	static bool newFolder = false;
	static bool newShader = false;
	static bool newMaterial = false;
	static bool deleteFile = false;
	if(ImGui::BeginPopupContextWindow("FileOptions")) {
		if(ImGui::MenuItem("New Folder")) {
			newFolder = true;
		}
		if(ImGui::MenuItem("New Shader")) {
			newShader = true;
		}
		if(ImGui::MenuItem("New Material")) {
			newMaterial = true;
		}
		if(ImGui::MenuItem("Delete")) {
			deleteFile = true;
		}
		ImGui::EndPopup();
	}

	//New Folder dialog
	if(newFolder) {
		ImGui::OpenPopup("Folder Name");
		newFolder = false;
	}
	if(ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buffer[256];
		ImGui::Text("Enter folder name");
		if(!ImGui::IsAnyItemActive()) {
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("##FolderName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) ||
			input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) ||
			input->IsPressed(Key::ENTER)) {

			os->CreateDirectory(os->AssetsPath() + current_path + buffer);

			Refresh();

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//New Shader dialog
	if(newShader) {
		ImGui::OpenPopup("New Shader");
		newShader = false;
	}
	if(ImGui::BeginPopupModal("New Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buffer[256];
		ImGui::Text("Enter Shader name");
		ImGui::InputText("##ShaderName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) ||
		   input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {

			Shader* shader = CREATE Shader();
			shader->Save(current_path + "//" + String(buffer) + ".sha");
			delete shader;

			Refresh();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	//New Material dialog
	if(newMaterial) {
		ImGui::OpenPopup("New Material");
		newMaterial = false;
	}
	if(ImGui::BeginPopupModal("New Material", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		//Material name
		static char buffer[256];
		ImGui::Text("Enter Material name");
		ImGui::InputText("##MaterialName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) || input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {
			Material* material = CREATE Material();
			material->Save(current_path + "//" + String(buffer) + ".mat");

			delete material;

			Refresh();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	//Delete dialog
	if(deleteFile) {
		ImGui::OpenPopup("Delete?");
		deleteFile = false;
	}
	if(ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you what to delete this file?");
		if(ImGui::Button("OK", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup(); 
			os->Delete(current_path);
			current_path = os->AssetsPath();
			FileSelected.Emit(current_path);
			Refresh();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if(ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void FilesView::OnScreenChanged(Screen* screen) {
	current_path = os->AssetsPath();
	FileSelected.Emit(current_path);
}
