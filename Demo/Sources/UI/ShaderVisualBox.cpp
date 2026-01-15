#include "ShaderVisualBox.h"
#include "Shaders/Shader.h"
#include "File.h"
#include "Demo.h"
#include "System.h"
#include "Graphics.h"
#include "FileSelector.h"
#include "Base/Factory.h"
#include "Views/FilesView.h"
 
#include "ThirdParty/ImGui/imgui.h"

ShaderVisualBox::ShaderVisualBox(FilesView* filesView) {
	filesView->FileSelected.Connect(this, &ShaderVisualBox::OnFileSelected);
	vertex_file_selector = CREATE FileSelector(filesView, "Vertex File", "vert");
	fragment_file_selector = CREATE FileSelector(filesView, "Fragment File", "frag");
	for(int i = 0; i < Shader::Uniform::Type::UNKNOWN; i++) {
		Shader::Uniform::Type type = (Shader::Uniform::Type)i;
		type_names.Add(Shader::Uniform::TypeToString(type));
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
		ImVec2 textSize = ImGui::CalcTextSize("Shader");
		ImGui::SameLine(ImGui::GetWindowWidth() / 2.f - textSize.x / 2.f);
		ImGui::Text("Shader");
		ImGui::PopFont();
		
		String shaderClassName = gfx->shader_factory->GetNameByClass(shader);
		ImGui::TextUnformatted("Shader Class: " + shaderClassName);

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

		//Uniforms block 
		ImGui::NewLine();
		ImGui::Text("Uniforms");
		ImGui::Separator();
		ImGui::Columns(3, "Uniforms", false);

		ImGui::Text("Name"); ImGui::NextColumn();
		ImGui::Text("glName"); ImGui::NextColumn();
		ImGui::Text("Type"); ImGui::NextColumn();
		ImGui::Separator();

		index = 0;
		for(int i = 0; i < shader->GetUniforms().Size(); i++) {
			Shader::Uniform& uni = shader->GetUniforms()[i];
			ImGui::PushID(String::Format("UniformNameID #", index++));

			buffer = uni.name;
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##Name", buffer.ToCStr(), buffer.Capacity());
			uni.name = buffer.ToCStr();

			ImGui::NextColumn();
			buffer = uni.glName;
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##GlName", buffer.ToCStr(), buffer.Capacity());
			uni.glName = buffer.ToCStr();
			ImGui::NextColumn();

			char* values[Shader::Uniform::Type::UNKNOWN];
			for(S32 i = 0; i < type_names.Size(); i++) {
				values[i] = type_names[i].ToCStr();
			}

			availableWidth = ImGui::GetColumnWidth();
			ImGui::PushItemWidth(availableWidth - SCALED(35.f));
			if(ImGui::BeginCombo("##Type", Shader::Uniform::TypeToString(uni.type))) {
				for(int i = 0; i < Shader::Uniform::Type::UNKNOWN; i++) {
					Shader::Uniform::Type type = (Shader::Uniform::Type)i;
					bool selected = type == uni.type;
					if(ImGui::Selectable(values[i], selected)) {
						uni.type = type;
					}
				}

				ImGui::EndCombo();
			}
			ImGui::SameLine(availableWidth - SCALED(20.f));
			if(ImGui::Button("-", ImVec2(-1, 0))) {
				shader->GetUniforms().Remove(i);
				i--;
			}

			ImGui::NextColumn();
			ImGui::PopID();
		}
		ImGui::Columns(1);

		ImGui::PushItemWidth(-1);
		if(ImGui::Button("New Uniform", ImVec2(-1, 0))) {
			Shader::Uniform newUniform("NewUniform", "glNewUniform", Shader::Uniform::Type::INT);
			shader->AddUniform(newUniform);
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
			shader->Save(os->AssetsPath() + shader_filename, shaderClassName);
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
