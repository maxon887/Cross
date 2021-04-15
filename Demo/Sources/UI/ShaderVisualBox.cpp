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
#include "ShaderVisualBox.h"
#include "Shaders/Shader.h"
#include "File.h"
#include "Demo.h"
#include "System.h"

#include "ThirdParty/ImGui/imgui.h"

ShaderVisualBox::ShaderVisualBox() {
	for(int i = 0; i < Shader::Property::Type::UNKNOWN; i++) {
		Shader::Property::Type type = (Shader::Property::Type)i;
		type_names.Add(Shader::Property::TypeToString(type));
	}
}

ShaderVisualBox::~ShaderVisualBox() {
	delete shader;
}

void ShaderVisualBox::Update() {
	if(shader) {
		ImVec2 spacing = ImGui::GetStyle().ItemInnerSpacing;
		spacing.x = 0;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, spacing);

		ImGui::PushFont(demo->big_font);
		ImVec2 textSize = ImGui::CalcTextSize("Shader File");
		ImGui::SameLine(ImGui::GetWindowWidth() / 2.f - textSize.x / 2.f);
		ImGui::Text("Shader File");
		ImGui::PopFont();

		//vertex file
		ImGui::Text("Vertex File:");
		ImGui::SameLine(SCALED(120.f));
		String vertexFile = shader->GetVertexFilename();
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), vertexFile);

		//fragment file
		ImGui::Text("Fragment File:");
		ImGui::SameLine(SCALED(120.f));
		String fragmentFile = shader->GetFragmentFilename();
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), fragmentFile);

		ImGui::NewLine();

		ImGui::Text("Properties");
		ImGui::Separator();
		ImGui::Columns(3, "Properties", false);

		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("glName"); ImGui::NextColumn();
		ImGui::Text("Type"); ImGui::NextColumn();
		ImGui::Separator();

		String buffer("", 0, 256);
		for(Shader::Property& prop : shader->GetProperties()) {
			buffer = prop.GetName();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##Name", buffer.ToCStr(), buffer.Capacity());
			ImGui::NextColumn();
			buffer = prop.GetGLName();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##GlName", buffer.ToCStr(), buffer.Capacity());
			ImGui::NextColumn();

			char* values[Shader::Property::Type::UNKNOWN];
			for(S32 i = 0; i < type_names.Size(); i++) {
				values[i] = type_names[i].ToCStr();
			}

			float availableWidth = ImGui::GetColumnWidth();
			ImGui::PushItemWidth(availableWidth - SCALED(35.f));
			if(ImGui::BeginCombo("##" + prop.GetName(), Shader::Property::TypeToString(prop.GetType()))) {

				for(int i = 0; i < Shader::Property::Type::UNKNOWN; i++) {
					Shader::Property::Type type = (Shader::Property::Type)i;
					bool selected = type == prop.GetType();
					if(ImGui::Selectable(values[i], selected)) {

					}
					//	current_item_2 = items[n];
					//if (is_selected)
					//	ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}

				ImGui::EndCombo();
			}
			ImGui::SameLine(availableWidth - SCALED(20.f));
			if(ImGui::Button("-", ImVec2(-1, 0))) {

			}

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::PushItemWidth(-1);
		if(ImGui::Button("New Property", ImVec2(-1, 0))) {

		}

		float availableWidth = ImGui::GetWindowWidth();
		ImGui::NewLine();
		ImGui::SameLine(availableWidth / 2);
		if(ImGui::Button("Revert", ImVec2(availableWidth / 4 - SCALED(10.f), 0))) {

		}
		//ImGui::SameLine(availableWidth / 2 + SCALED(110.f));
		ImGui::SameLine(availableWidth / 4 * 3);
		if(ImGui::Button("Save", ImVec2(-1, 0))) {

		}

		ImGui::PopStyleVar();
	}
}

void ShaderVisualBox::OnFileSelected(String filename) {
	delete shader;
	if(File::ExtensionFromFile(filename) == "sha") {
		shader = new Shader();
		shader->Load(filename);
	} else {
		shader = nullptr;
	}
}
