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
		type_names.push_back(Shader::Property::TypeToString(type));
	}
}

ShaderVisualBox::~ShaderVisualBox() {
	delete shader;
}

void ShaderVisualBox::Update() {
	if(shader) {
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

		ImGui::Text("Properties");
		ImGui::Separator();
		ImGui::Columns(3, "ads", false);  // 3-ways, no border

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
			for(U32 i = 0; i < type_names.size(); i++) {
				values[i] = type_names[i].ToCStr();
			}

			ImGui::PushItemWidth(-1);
			if(ImGui::BeginCombo(String("##" + prop.GetName()).ToCStr(), Shader::Property::TypeToString(prop.GetType()))) {

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
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
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
