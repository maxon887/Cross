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
#pragma once
#include "MaterialVisualBox.h"
#include "Material.h"
#include "File.h"
#include "Demo.h"
#include "System.h"
#include "Scene.h"

#include "ThirdParty/ImGui/imgui.h"

MaterialVisualBox::~MaterialVisualBox() {
	if(!loaded_from_scene && mat) {
		delete mat->GetShader();
		delete mat;
	}
}

void MaterialVisualBox::Update() {
	if(mat) {
		ImGui::PushFont(demo->big_font);
		ImGui::SameLine(ImGui::GetWindowWidth() / 3);
		ImGui::Text("Material");
		ImGui::PopFont();

		ImGui::Text("Shader:");
		ImGui::SameLine(SCALED(70.f));
		String shaderFilename = mat->GetShader()->GetFilename();
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), shaderFilename);

		ImGui::NewLine();
		ImGui::Text("Properties");
		ImGui::Separator();

		for(Shader::Property& prop : mat->GetProperties()) {
			ImGui::Text(prop.GetName());
			ImGui::SameLine(ImGui::GetWindowWidth() / 3.f);
			switch(prop.GetType()) {
			case Shader::Property::Type::COLOR:
				ImGui::ColorEdit4(prop.GetName(), prop.GetValue().color.GetData(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				break;
			default:
				CROSS_ASSERT(false, "Can not display material property of type #", Shader::Property::TypeToString(prop.GetType()));
				break;
			}
		}

		float availableWidth = ImGui::GetWindowWidth();
		ImGui::NewLine();
		ImGui::SameLine(availableWidth / 2);
		if(ImGui::Button("Revert", ImVec2(availableWidth / 4 - SCALED(10.f), 0))) {

		}
		ImGui::SameLine(availableWidth / 4 * 3);
		if(ImGui::Button("Save", ImVec2(-1, 0))) {

		}
	}
}

void MaterialVisualBox::OnFileSelected(String filename) {
	if(!loaded_from_scene && mat) {
		delete mat->GetShader();
		delete mat;
	}
	if(File::ExtensionFromFile(filename) == "mat") {
		Scene* scene = game->GetCurrentScene();
		if(scene) {
			mat = scene->GetMaterial(filename);
			loaded_from_scene = true;
		} else {
			mat = new Material();
			mat->Load(filename, nullptr);
			loaded_from_scene = false;
		}
	} else {
		mat = nullptr;
	}
}

void MaterialVisualBox::OnScreenChanged(Screen* newScreen) {
	if(loaded_from_scene) {
		mat = nullptr;
	}
}