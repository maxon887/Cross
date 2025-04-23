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
#include "MaterialVisualBox.h"
#include "Material.h"
#include "File.h"
#include "Demo.h"
#include "System.h"
#include "Scene.h"
#include "FileSelector.h"
#include "Graphics.h"

#include "ThirdParty/ImGui/imgui.h"

using namespace std;

MaterialVisualBox::MaterialVisualBox() {
	shader_selector = CREATE FileSelector("Shader", "sha");
	shader_selector->FileSelected.Connect(this, &MaterialVisualBox::OnShaderSelected);
}

MaterialVisualBox::~MaterialVisualBox() {
	delete shader_selector;
	DeleteMaterialIfNeeded();
	for(pair<String, FileSelector*> textureSelector : texture_selectors) {
		delete textureSelector.second;
	}
}

void MaterialVisualBox::Update() {
	if(mat) {
		ImGui::PushFont(demo->big_font);
		ImGui::SameLine(ImGui::GetWindowWidth() / 3);
		ImGui::Text("Material");
		ImGui::PopFont();

		shader_selector->Update();

		bool trans = mat->IsTransparent();
		if(ImGui::Checkbox("Transparent", &trans)) {
			mat->SetTransparent(trans);
		}

		ImGui::NewLine();
		ImGui::Text("Properties");
		ImGui::Separator();

		for(Shader::Property& prop : mat->GetProperties()) {
			switch(prop.type) {
			case Shader::Property::Type::INT: {
				ImGui::TextUnformatted(prop.name);
				ImGui::SameLine(ImGui::GetWindowWidth() / 3.f);
				ImGui::DragInt("##IntProperty", &prop.GetValue().s32);
				break;
			}
			case Shader::Property::Type::FLOAT: {
				ImGui::TextUnformatted(prop.name);
				ImGui::SameLine(ImGui::GetWindowWidth() / 3.f);
				ImGui::DragFloat("##FloatProperty", &prop.GetValue().f, 0.1f);
				break;
			}
			case Shader::Property::Type::COLOR: {
				ImGui::TextUnformatted(prop.name);
				ImGui::SameLine(ImGui::GetWindowWidth() / 3.f);
				ImGui::ColorEdit4(prop.name, prop.GetValue().color.GetData(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				break;
			}
			case Shader::Property::Type::TEXTURE: {
				FileSelector* fileSelector = texture_selectors[prop.name];
				if(fileSelector->Update()) {
					String textureFilename = fileSelector->GetSelectedFile();
					//textureFilename = File::FromAbsoluteToAssetPath(textureFilename);
					if(game->GetCurrentScene()) {
						prop.GetValue().texture = game->GetCurrentScene()->GetTexture(textureFilename);
					} else {
						Texture* texture = prop.GetValue().texture;
						delete texture;
						texture = CREATE Texture();
						texture->Load(textureFilename);
						prop.GetValue().texture = texture;
					}
				}
				break;
			}
			default:
				CROSS_ASSERT(false, "Can not display material property of type #", Shader::Property::TypeToString(prop.type));
				break;
			}
		}

		float availableWidth = ImGui::GetWindowWidth();
		ImGui::NewLine();
		ImGui::SameLine(availableWidth / 2);
		if(ImGui::Button("Revert", ImVec2(availableWidth / 4 - SCALED(10.f), 0))) {
			if(loaded_from_scene) {
				mat->Load(mat->GetFilename(), game->GetCurrentScene());
			} else {
				mat->Load(mat->GetFilename(), nullptr);
			}
			if(mat->GetShader()) {
				shader_selector->SetSelectedFile(mat->GetShader()->GetFilename());
			}
		}
		ImGui::SameLine(availableWidth / 4 * 3);
		if(ImGui::Button("Save", ImVec2(-1, 0))) {
			mat->Save(os->AssetsPath() + mat->GetFilename());
		}
	}
}

void MaterialVisualBox::OnFileSelected(const String& filename) {
	DeleteMaterialIfNeeded();
	if(File::ExtensionFromFile(filename) == "mat") {
		Scene* scene = game->GetCurrentScene();
		if(scene) {
			mat = scene->GetMaterial(filename);
			loaded_from_scene = true;
		} else {
			mat = CREATE Material();
			bool success = mat->Load(filename, nullptr);
			loaded_from_scene = false;
			if(!success) {
				delete mat;
				mat = nullptr;
			}
		}
		if(mat->GetShader()) {
			shader_selector->SetSelectedFile(mat->GetShader()->GetFilename());
		} else {
			shader_selector->SetSelectedFile("");
		}
		CreateTextureSelectors();
	} else {
		mat = nullptr;
	}
}

void MaterialVisualBox::OnShaderSelected(const String& filename) {
	Shader* oldMaterialShader = mat->GetShader();
	if(!filename.IsEmpty() && (!oldMaterialShader || filename != oldMaterialShader->GetFilename())) {
		if(!loaded_from_scene) {
			delete oldMaterialShader;
		}
		Shader* newShader = gfx->LoadShader(filename);
		if(loaded_from_scene && !newShader->IsCompiled()) {
			newShader->Compile();
		}
		mat->SetShader(newShader);
		CreateTextureSelectors();
	}
}

void MaterialVisualBox::OnScreenChanged(Screen* newScreen) {
	if(loaded_from_scene) {
		mat = nullptr;
	}
}

void MaterialVisualBox::DeleteMaterialIfNeeded() {
	if(!loaded_from_scene && mat) {
		for(Shader::Property& prop : mat->GetProperties()) {
			if(prop.type == Shader::Property::Type::TEXTURE) {
				delete prop.GetValue().texture;
			}
		}
		delete mat->GetShader();
		delete mat;
	}
}

void MaterialVisualBox::CreateTextureSelectors() {
	for(pair<String, FileSelector*> textureSelector : texture_selectors) {
		delete textureSelector.second;
	}
	texture_selectors.clear();
	for(Shader::Property& prop : mat->GetProperties()) {
		if(prop.type == Shader::Property::Type::TEXTURE) {
			FileSelector* textureSelector = CREATE FileSelector(prop.name, "png");
			texture_selectors[prop.name] = textureSelector;
			if(prop.GetValue().texture) {
				textureSelector->SetSelectedFile(prop.GetValue().texture->GetName());
			}
		}
	}
}