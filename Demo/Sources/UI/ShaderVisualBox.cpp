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
#include "Graphics.h"
#include "FileSelector.h"
 
#include "ThirdParty/ImGui/imgui.h"

ShaderVisualBox::ShaderVisualBox() {
	vertex_file_selector = CREATE FileSelector("Vertex File", "vert");
	fragment_file_selector = CREATE FileSelector("Fragment File", "frag");
	for(int i = 0; i < Shader::Property::Type::UNKNOWN; i++) {
		Shader::Property::Type type = (Shader::Property::Type)i;
		type_names.Add(Shader::Property::TypeToString(type));
	}
}

ShaderVisualBox::~ShaderVisualBox() {
	delete shader;
	delete vertex_file_selector;
	delete fragment_file_selector;
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

		vertex_file_selector->Update();
		fragment_file_selector->Update();

		//Macrosies block
		ImGui::NewLine();
		float availableWidth = ImGui::GetColumnWidth();
		ImGui::Text("Macrosies");
		ImGui::Separator();
		int index = 0;
		String buffer("", 0, 256);
		for(int i = 0; i < shader->GetMacrosies().Size(); i++) {
			String& macro = shader->GetMacrosies()[i];
			ImGui::PushItemWidth(availableWidth - SCALED(35.f));
			ImGui::PushID(String::Format("MacroEdit #", index++));
			buffer = macro;
			ImGui::InputText("##Macro", buffer.ToCStr(), buffer.Capacity());
			macro = buffer.ToCStr();
			ImGui::SameLine(availableWidth - SCALED(20.f));

			if(ImGui::Button("-", ImVec2(-1, 0))) {
				shader->GetMacrosies().Remove(i);
				i--;
			}
			ImGui::PopID();
		}

		if(ImGui::Button("New Macro", ImVec2(-1, 0))) {
			String newMacroString("", 0, 256);
			newMacroString += "NEW_MACRO";
			shader->GetMacrosies().Add(newMacroString);
		}

		//Properties block 
		ImGui::NewLine();
		ImGui::Text("Properties");
		ImGui::Separator();
		ImGui::Columns(3, "Properties", false);

		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("glName"); ImGui::NextColumn();
		ImGui::Text("Type"); ImGui::NextColumn();
		ImGui::Separator();

		index = 0;
		for(int i = 0; i < shader->GetProperties().Size(); i++) {
			Shader::Property& prop = shader->GetProperties()[i];
			ImGui::PushID(String::Format("PropertyNameID #", index++));

			buffer = prop.name;
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##Name", buffer.ToCStr(), buffer.Capacity());
			prop.name = buffer.ToCStr();

			ImGui::NextColumn();
			buffer = prop.glName;
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##GlName", buffer.ToCStr(), buffer.Capacity());
			prop.glName = buffer.ToCStr();
			ImGui::NextColumn();

			char* values[Shader::Property::Type::UNKNOWN];
			for(S32 i = 0; i < type_names.Size(); i++) {
				values[i] = type_names[i].ToCStr();
			}

			availableWidth = ImGui::GetColumnWidth();
			ImGui::PushItemWidth(availableWidth - SCALED(35.f));
			if(ImGui::BeginCombo("##Type", Shader::Property::TypeToString(prop.type))) {

				for(int i = 0; i < Shader::Property::Type::UNKNOWN; i++) {
					Shader::Property::Type type = (Shader::Property::Type)i;
					bool selected = type == prop.type;
					if(ImGui::Selectable(values[i], selected)) {
						prop.type = type;
					}
				}

				ImGui::EndCombo();
			}
			ImGui::SameLine(availableWidth - SCALED(20.f));
			if(ImGui::Button("-", ImVec2(-1, 0))) {
				shader->GetProperties().Remove(i);
				i--;
			}

			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::PushItemWidth(-1);
		if(ImGui::Button("New Property", ImVec2(-1, 0))) {
			Shader::Property newProperty("NewProperty", "glNewProperty", Shader::Property::Type::INT);
			shader->AddProperty(newProperty);
		}

		availableWidth = ImGui::GetWindowWidth();
		ImGui::NewLine();
		ImGui::SameLine(availableWidth / 2);
		if(ImGui::Button("Revert", ImVec2(availableWidth / 4 - SCALED(10.f), 0))) {
			OnFileSelected(shader_filename);
		}
		ImGui::SameLine(availableWidth / 4 * 3);
		if(ImGui::Button("Save", ImVec2(-1, 0))) {
			shader->SetVertexFilename(vertex_file_selector->GetSelectedFile());
			shader->SetFragmentFilename(fragment_file_selector->GetSelectedFile());
			shader->Save(os->AssetsPath() + shader_filename);
		}

		ImGui::PopStyleVar();
	}
}

void ShaderVisualBox::OnFileSelected(const String& filename) {
	delete shader;
	if(File::ExtensionFromFile(filename) == "sha") {
		shader_filename = filename;
		shader = gfx->LoadShader(filename);
		vertex_file_selector->SetSelectedFile(shader->GetVertexFilename());
		fragment_file_selector->SetSelectedFile(shader->GetFragmentFilename());
	} else {
		shader = nullptr;
	}
}
