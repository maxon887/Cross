#include "FilesView.h"
#include "System.h"
#include "Graphics.h"
#include "File.h"
#include "Game.h"
#include "Demo.h"
#include "Material.h"
#include "Scenes/DemoScene.h"
#include "FileUtils.h"
#include "Input.h"
#include "Base/Factory.h"

#include <algorithm>

#include "ThirdParty/ImGui/imgui.h"

#undef CreateDirectory

using namespace std;

FilesView::FilesView() : View("Files", true) { }

void FilesView::Shown() {
	if(!file_tree.initialized) {
		InitNode(file_tree);
	}
	current_path = "";
	game->ScreenChanged.Connect(this, &FilesView::OnScreenChanged);
}

void FilesView::Update(float sec) {
	BuildNote(file_tree);

	if(ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
		current_path = "";
		FileSelected.Emit(current_path);
	}

	ContextMenu();
}

void FilesView::AskToShowFile(const String& filename) {
	CROSS_FAIL(os->IsFileExists(os->AssetsPath() + filename), "Can not show file: '#'\nProbably file does not exists", filename);
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
	String extension = File::ExtensionFromFile(filename);
	if(extension == "scn") {
		Scene* scene = CREATE DemoScene();
		if(!scene->Load(filename)) {
			delete scene;
		} else {
			game->SetScreen(scene);
		}
	} else if(extension == "fbx" || extension == "FBX") {
		DemoScene* scene = dynamic_cast<DemoScene*>(game->GetCurrentScene());
		if(scene) {
			scene->ImportModel(filename);
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
	bool newFolder = false;
	bool newShader = false;
	bool newMaterial = false;
	bool deleteFile = false;
	bool somethingSelected = !current_path.IsEmpty();
	
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
		if(ImGui::MenuItem("Delete", "del, backspace", false, somethingSelected)) {
			deleteFile = true;
		}
		ImGui::EndPopup();
	}
	if((input->IsPressed(Key::DEL) || input->IsPressed(Key::BACKSPACE)) && ImGui::IsWindowFocused() && somethingSelected) {
		deleteFile = true;
	}

	//New Folder dialog
	if(newFolder) {
		ImGui::OpenPopup("Folder Name");
	}
	if(ImGui::BeginPopupModal("Folder Name", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buffer[256];
		ImGui::Text("Enter folder name");
		if(!ImGui::IsAnyItemActive()) {
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("##FolderName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) || input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {
			os->CreateDirectory(os->AssetsPath() + current_path + buffer);
			Refresh();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	//New Shader dialog
	if(newShader) {
		ImGui::OpenPopup("New Shader");
	}
	if(ImGui::BeginPopupModal("New Shader", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char buffer[256];
		ImGui::Text("Enter Shader name");
		if(!ImGui::IsAnyItemActive() && ImGui::IsWindowAppearing()) {
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("##ShaderName", buffer, 256);

		Array<String> shaderNames = gfx->shader_factory->GetRegisteredComponentsName();
		static int selectableIndex = 0;
		if(ImGui::BeginCombo("Shader Class", shaderNames[selectableIndex])) {
			for(int i = 0; i < shaderNames.Size(); i++) {
				const String& shaderClass = shaderNames[i];
				bool selected = shaderClass == shaderNames[selectableIndex];
				if(ImGui::Selectable(shaderClass, selected)) {
					selectableIndex = i;
				}
			}
			ImGui::EndCombo();
		}
		const String& selectedShaderClass = shaderNames[selectableIndex];

		if(ImGui::Button("Cancel", ImVec2(120, 0)) ||
		   input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {
			if(os->IsFileExists(os->AssetsPath() + current_path)) { //we are currently looking at file. we need to obtain path from it
				current_path = File::PathFromFile(current_path);
			}
			current_path += String(buffer) + ".sha";

			Shader* shader = gfx->shader_factory->Create(selectedShaderClass);
			shader->Save(os->AssetsPath() + current_path, selectedShaderClass);
			delete shader;

			Refresh();
			ImGui::CloseCurrentPopup();
			FileSelected.Emit(current_path);
			FileTreeChanged.Emit();
		}
		ImGui::EndPopup();
	}

	//New Material dialog
	if(newMaterial) {
		ImGui::OpenPopup("New Material");
	}
	if(ImGui::BeginPopupModal("New Material", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		//Material name
		static char buffer[256];
		ImGui::Text("Enter Material name");
		if(!ImGui::IsAnyItemActive()) {
			ImGui::SetKeyboardFocusHere(0);
		}
		ImGui::InputText("##MaterialName", buffer, 256);

		if(ImGui::Button("Cancel", ImVec2(120, 0)) || input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if(ImGui::Button("Ok", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {
			if(os->IsFileExists(os->AssetsPath() + current_path)) { //we are currently looking at file. we need to obtain path from it
				current_path = File::PathFromFile(current_path);
			}
			current_path += String(buffer) + ".mat";
						
			Material* material = CREATE Material();
			material->Save(os->AssetsPath() + current_path);
			delete material;

			Refresh();
			ImGui::CloseCurrentPopup();
			FileSelected.Emit(current_path);
			FileTreeChanged.Emit();
		}

		ImGui::EndPopup();
	}
	//Delete dialog
	if(deleteFile) {
		ImGui::OpenPopup("Delete?");
	}
	if(ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you what to delete this content?");
		if(ImGui::Button("OK", ImVec2(120, 0)) || input->IsPressed(Key::ENTER)) {
			ImGui::CloseCurrentPopup(); 
			os->Delete(os->AssetsPath() + current_path);
			current_path = "";
			FileSelected.Emit(current_path);
			FileTreeChanged.Emit();
			Refresh();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if(ImGui::Button("Cancel", ImVec2(120, 0)) || input->IsPressed(Key::ESCAPE)) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void FilesView::OnScreenChanged(Screen* screen) {
	current_path = "";
	FileSelected.Emit(current_path);
}

bool FilesView::AvailableInMenu() {
	return !os->IsMobile();
}
