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

void MeshVisualBox::Show(Mesh* mesh) {
	ImGui::Text("Mesh ID:");
	ImGui::SameLine(SCALED(100.f));
	ImGui::PushItemWidth(SCALED(70.f));

	ImGui::DragInt("## Mesh ID", &MeshID, 0.1f, -1, MeshIDRange);
	if(ImGui::IsItemDeactivated() && MeshID != mesh->GetID()) {
		mesh->SetID(MeshID);
		if(!mesh->GetModelFileName().IsEmpty()) {
			Model* newModel = game->GetCurrentScene()->GetModel(mesh->GetModelFileName());
			Mesh* downloadedMesh = newModel->GetMesh(MeshID);
			if(downloadedMesh) {
				mesh->TransferVideoData(downloadedMesh);
			} else {
				mesh->Disable();
				mesh->SetID(-1);
			}
		}
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(SCALED(6.f), SCALED(6.f)));

	//model filename
	ImGui::Text("Model file:");
	ImGui::SameLine(SCALED(100.f));
	String modelName = mesh->GetModelFileName();
	modelName = File::FileFromPath(modelName);
	if(ImGui::Button(modelName)) {
		modelName = system->OpenFileDialog();
		if(!modelName.IsEmpty()) {
			
			Model* newModel = game->GetCurrentScene()->GetModel(modelName);
			if(newModel) {
				mesh->SetModelFileName(modelName);
				MeshIDRange = newModel->GetMeshesCount() - 1;
			} else {
				CROSS_ASSERT(false, "Can not load model #", mesh->GetModelFileName());
			}
		}
	}

	//material filename
	ImGui::Text("Material:");
	ImGui::SameLine(SCALED(100.f));
	String materialFilename = mesh->GetMaterialFileName();
	materialFilename = File::FileFromPath(materialFilename);
	if(ImGui::Button(materialFilename)) {
		materialFilename = system->OpenFileDialog();
		if(!materialFilename.IsEmpty()) {
			Material* selectedMaterial = game->GetCurrentScene()->GetMaterial(materialFilename);
			mesh->SetMaterial(selectedMaterial);
		}
	}

	ImGui::PopStyleVar();
}

void MeshVisualBox::EntitySelected(Entity* entity) {
	if(entity) {
		Mesh* mesh = entity->GetComponent<Mesh>();
		if(mesh) {
			MeshID = mesh->GetID();
			Model* model = game->GetCurrentScene()->GetModel(mesh->GetModelFileName());
			MeshIDRange = model->GetMeshesCount() - 1;
		}
	}
}