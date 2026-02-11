#include "MeshVisualBox.h"
#include "Demo.h"
#include "Mesh.h"
#include "File.h"
#include "Scene.h"
#include "Entity.h"

#include "ThirdParty/ImGui/imgui.h"

MeshVisualBox::MeshVisualBox(FilesView* filesView) {
	material_file = CREATE FileSelector(filesView, "Material", "mat");
	material_file->FileSelected.Connect(this, &MeshVisualBox::MaterialSelected);
}

MeshVisualBox::~MeshVisualBox() {
	material_file->FileSelected.Disconnect(this, &MeshVisualBox::MaterialSelected);
	delete material_file;	
}

void MeshVisualBox::Update() {
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(SCALED(6.f), SCALED(6.f)));

	//model filename
	ImGui::Text("Model:");
	ImGui::SameLine(SCALED(100.f));
	String modelFile = mesh->GetModelFileName();
	if(!modelFile.IsEmpty()) {
		modelFile = File::FileFromPath(modelFile);
	} else {
		modelFile = "Runtime model";
	}
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", modelFile.ToCStr());
	
	ImGui::Text("Mesh Group ID: ");
	ImGui::SameLine(SCALED(100.f));
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", mesh->GetID().ToCStr());
	
	material_file->Update();

	ImGui::PopStyleVar();
}

void MeshVisualBox::EntitySelected(Entity* newEntity) {
	if(!newEntity) {
		return;
	}
	mesh = newEntity->GetComponent<Mesh>();
	if(!mesh) {
		return;
	}
	String materialFilename = mesh->GetMaterialFileName();
	if(materialFilename.IsEmpty()) {
		material_file->SetText("Runtime Material");
	} else {
		material_file->SetSelectedFile(materialFilename);
	}
}

void MeshVisualBox::MaterialSelected(const String& materialFile) {
	if(materialFile != mesh->GetMaterialFileName()) {
		mesh->SetMaterial(game->GetCurrentScene()->GetMaterial(materialFile));
	}
}