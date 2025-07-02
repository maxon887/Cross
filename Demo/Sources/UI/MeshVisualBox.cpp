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
	ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%i", mesh->GetID());
	
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