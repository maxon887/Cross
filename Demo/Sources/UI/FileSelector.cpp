#include "FileSelector.h"
#include "FileUtils.h"
#include "File.h"
#include "Input.h"
#include "Demo.h"
#include "System.h"
#include "Views/FilesView.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_internal.h"

FileSelector::FileSelector(FilesView* filesView, const String &label, const String &fileExtension) {
	this->files_view = filesView;
	this->label = label;
	this->extension = fileExtension;
	filesView->FileTreeChanged.Connect(this, &FileSelector::RefreshAllAvailableFiles);
	RefreshAllAvailableFiles();
}

FileSelector::~FileSelector() {
	files_view->FileTreeChanged.Disconnect(this, &FileSelector::RefreshAllAvailableFiles);
}

bool FileSelector::Update() {
	bool fileSelected = false;
	float availableWidth = ImGui::GetWindowWidth();
	float availableHeight = ImGui::GetWindowHeight();
	float labelWidth = ImGui::CalcTextSize(label.ToCStr()).x + SCALED(10.f);
	
	ImGui::PushItemWidth(availableWidth - labelWidth - SCALED(74)); //74 = two buttons + spacing
	if(ImGui::InputText("##InputText" + label, current_input.ToCStr(), current_input.Capacity(), ImGuiInputTextFlags_AutoSelectAll)) {
		ValueChanged();
	}
	
	if(ImGui::IsItemHovered()) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 minRec = ImGui::GetItemRectMin();
		ImVec2 maxRec = ImGui::GetItemRectMax();
		maxRec.x = minRec.x + ImGui::CalcItemWidth();
		drawList->AddRect(minRec, maxRec, ImColor(0,0,255));
	}
	
	if(ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FilesViewDRAG")) {
			const String* filename = nullptr;
			memcpy(&filename, payload->Data, sizeof(String*));
			String relativePath = File::FromAbsoluteToAssetPath(*filename);
			if(CheckFileExtension(relativePath)) {
				SetSelectedFile(relativePath);
				fileSelected = true;
			}
		}
		ImGui::EndDragDropTarget();
	}
	
    const bool inputTextActive = ImGui::IsItemActive();
	String strID = "SuggestionPopup";
	strID += label;
	if(inputTextActive) {
		ImGui::OpenPopup(strID, ImGuiPopupFlags_NoReopen);
	}

	// Position and size popup
	ImVec2 popupPosition;
	ImVec2 winPos = ImGui::GetWindowPos();
	popupPosition.x = ImGui::GetItemRectMin().x;
	popupPosition.y = ImGui::GetItemRectMax().y + ImGui::GetStyle().ItemSpacing.y;
	ImGui::SetNextWindowPos(popupPosition);
	ImGui::SetNextWindowSize({ GImGui->LastItemData.NavRect.GetWidth(), availableHeight - (popupPosition.y - winPos.y) - SCALED(10.f) }, ImGuiCond_Appearing);
	
	ImGuiWindowFlags popupWindowFlags = ImGuiWindowFlags_NoTitleBar;
	popupWindowFlags |= ImGuiWindowFlags_NoMove;
	popupWindowFlags |= ImGuiWindowFlags_NoSavedSettings;
	popupWindowFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
	popupWindowFlags |= ImGuiWindowFlags_ChildWindow;
	popupWindowFlags |= ImGuiWindowFlags_NoNav;
	if (ImGui::BeginPopupEx(ImGui::GetID(strID), popupWindowFlags)) {
		for(int i = 0; i < suggested_names.Size(); i++) {
			const String& potentialFile = suggested_names[i];
			bool Selected = i == suggested_index;
			if(ImGui::Selectable(potentialFile.ToCStr(), Selected)) {
				//file selected
				S32 foundFileIndex = all_names.Find(potentialFile);
				String newFile = files_list[foundFileIndex];
				SetSelectedFile(newFile);
				fileSelected = true;
			}
		}
		static bool justPressed = false;
		if(input->IsPressed(Key::TAB)) {
			if(!justPressed) {
				suggested_index = (suggested_index + 1) % suggested_names.Size();
			}
			justPressed = true;
		} else {
			justPressed = false;
		}
		if(input->IsPressed(Key::ENTER)) {
			String suggestedFile = suggested_names[suggested_index];
			for(int i = 0; i < all_names.Size(); i++) {
				const String& name = all_names[i];
				if(suggestedFile == name) {
					SetSelectedFile(files_list[i]);
					fileSelected = true;
				}
			}
		}
		// Close popup on deactivation (unless we are mouse-clicking in our popup)
        if (!inputTextActive && !ImGui::IsWindowFocused())
            ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	
	ImGui::SameLine(availableWidth - labelWidth);
	ImGui::Text("%s", label.ToCStr());
	
	ImGui::SameLine(availableWidth - SCALED(32) - labelWidth);
	ImGui::PushID(label + "Locate Button");
	if(ImGui::Button("?", ImVec2(SCALED(26), 0))) {
		//try to find file in FilesView
		files_view->AskToShowFile(selected_file);
	}
	ImGui::PopID();
	
	ImGui::SameLine(availableWidth - SCALED(64) - labelWidth);
	ImGui::PushID(label + "Add Button");
	if(ImGui::Button("+", ImVec2(SCALED(26), 0))) {
		String filename = os->OpenFileDialog();
		if(!filename.IsEmpty()) {
			SetSelectedFile(filename);
			fileSelected = true;
		}
	}
	ImGui::PopID();
	return fileSelected;
}

String FileSelector::GetSelectedFile() const {
	return selected_file;
}

void FileSelector::SetSelectedFile(const String &filename) {
	selected_file = filename;
	current_input = File::FileFromPath(filename);
	current_input.Reserve(255);
	ValueChanged();
	FileSelected.Emit(filename);
}

void FileSelector::SetText(const String &messageText) {
	current_input = messageText;
}

void FileSelector::ValueChanged() {
	if(current_input == File::FileFromPath(selected_file)) {
		suggested_names = all_names;
	} else {
		suggested_index = 0;
		String agnosticInput = current_input.ToCStr(); //hack there because current_input have wrong length
		agnosticInput.Lowercase();
		suggested_names.Clear();
		for(const String& filename : all_names) {
			String agnosticFilename = filename;
			agnosticFilename.Lowercase();
			if(agnosticFilename.Find(agnosticInput) != -1) {
				suggested_names.Add(filename);
			}
		}
	}
}

bool FileSelector::CheckFileExtension(const String& filename) {
	S32 result = filename.Find(extension);
	if(result != -1) {
		return true;
	}
	return false;
}

void FileSelector::RefreshAllAvailableFiles() {
	files_list = FileUtils::GetAllFilesOfType(extension);\
	all_names.Clear();
	for(const String& filename : files_list) {
		all_names.Add(File::FileFromPath(filename));
	}
	ValueChanged();
}