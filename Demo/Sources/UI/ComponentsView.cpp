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
#include "ComponentsView.h"
#include "SceneView.h"
#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Demo.h"
#include "ComponentFactory.h"

#include "ThirdParty/ImGui/imgui.h"

ComponentsView::ComponentsView(SceneView* sceneView) :
	View("Components")
{
	scene_view = sceneView;
	scene_view->EntitySelected.Connect(&tranform_box, &TransformVisualBox::EntitySelected);
	//scene_view->EntitySelected.Connect(&mesh_box, &MeshVisualBox::EntitySelected);
}

void ComponentsView::Update(float sec) {
	Entity* entity = scene_view->GetSelectedEntity();
	if(entity) {
		for(Component* component : entity->GetComponents()) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SCALED(4.f), SCALED(4.f)));

			bool open = ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_AllowItemOverlap);
			ImGui::SameLine(ImGui::GetWindowWidth() - 35);
			String checkboxHashName = "##EnableCheckbox" + component->GetName();
			bool enabled = component->IsEnabled();
			if(ImGui::Checkbox(checkboxHashName.ToCStr(), &enabled)) {
				if(enabled) {
					component->Enable();
				} else {
					component->Disable();
				}
			}

			if(open) {
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + SCALED(2.f));
				ImGui::Separator();

				if(dynamic_cast<Transform*>(component)) {
					tranform_box.Show((Transform*)component);
				} else if(dynamic_cast<Mesh*>(component)) {
					mesh_box.Show((Mesh*)component);
				} else {
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SCALED(1.f), SCALED(1.f)));
					ImGui::NewLine();
					ImGui::SameLine(SCALED(15.f));
					ImGui::BeginGroup();

					for(BaseProperty* prop : component->GetProperties()) {
						ShowProperty(prop);
					}

					ImGui::EndGroup();
					ImGui::PopStyleVar();
				}
				//Component general menu
				String tagName = "Options ##";
				tagName += (U64)component;
				if(ImGui::TreeNode(tagName)) {
					if(ImGui::MenuItem("Remove Component")) {
						entity->RemoveComponent(component);
					}
					ImGui::TreePop();
				}
				ImGui::Separator();
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + SCALED(8.f));
			}
			ImGui::PopStyleVar();
		}
	}

	ContextMenu(entity);
}

void ComponentsView::ShowProperty(BaseProperty* baseProperty) {
	if(dynamic_cast<Property<S32>*>(baseProperty)) {
		Property<S32>* prop = (Property<S32>*)baseProperty;
		ImGui::Text(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::PushItemWidth(SCALED(70.f));
		ImGui::DragInt("##" + prop->GetName(), &prop->value);
	} else if(dynamic_cast<Property<float>*>(baseProperty)) {
		Property<float>* prop = (Property<float>*)baseProperty;
		ImGui::Text(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::PushItemWidth(SCALED(100.f));
		ImGui::DragFloat("##" + prop->GetName(), &prop->value);
	} else if(dynamic_cast<Property<String>*>(baseProperty)) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(SCALED(6.f), SCALED(6.f)));

		Property<String>* prop = (Property<String>*)baseProperty;
		ImGui::Text(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), prop->value);

		ImGui::PopStyleVar();
	} else if(dynamic_cast<Property<Vector3D>*>(baseProperty)) {
		Property<Vector3D>* prop = (Property<Vector3D>*)baseProperty;
		ImGui::DragFloat3(prop->GetName(), (float*)prop->value.GetData(), 0.1f);
	} else if(dynamic_cast<Property<Quaternion>*>(baseProperty)) {
		Property<Quaternion>* prop = (Property<Quaternion>*)baseProperty;

		Vector3D axis = prop->value.GetAxis();
		float angle = prop->value.GetAngle();

		if(ImGui::DragFloat3("Axis", axis.GetData(), 0.1f)) {
			//transform->SetRotate(axis, angle);
		}
		if(ImGui::SliderFloat("Angle", &angle, 0.0f, 360.f)) {
			//transform->SetRotate(axis, angle);
		}

	} else {
		String errStr = String::Format("Can not draw property '#'", baseProperty->GetName());
		ImGui::TextColored(ImVec4(1.f, 0, 0, 1.f), "%s", errStr.ToCStr());
	}
}

void ComponentsView::ContextMenu(Entity* selectedEntity) {
	if(ImGui::BeginPopupContextWindow("ComponentsOptions")) {
		bool enabled = selectedEntity != nullptr;
		if(ImGui::BeginMenu("Add Component", enabled)) {
			ComponentFactory* factory = game->GetComponentFactory();
			for(const String& componentName : factory->GetRegisteredComponentsName()) {
				if(ImGui::MenuItem(componentName.ToCStr(), "", false)) {
					Component* newComponent = factory->Create(componentName);
					Entity* selectedEntity = scene_view->GetSelectedEntity();
					newComponent->Disable();
					if(selectedEntity->GetComponent(typeid(*newComponent).hash_code())) {
						system->Messagebox("Error", "Component already exists");
						delete newComponent;
					} else {
						selectedEntity->AddComponent(newComponent);
					}
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}
