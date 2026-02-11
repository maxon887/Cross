#include "ComponentsView.h"
#include "SceneView.h"
#include "Entity.h"
#include "Component.h"
#include "Mesh.h"
#include "Demo.h"
#include "Base/Factory.h"

#include "ThirdParty/ImGui/imgui.h"

ComponentsView::ComponentsView(SceneView* sceneView, FilesView* filesView) :
	View("Components", true), mesh_box(filesView)
{
	scene_view = sceneView;
	scene_view->EntitySelected.Connect(&mesh_box, &MeshVisualBox::EntitySelected);
}

ComponentsView::~ComponentsView() {
	scene_view->EntitySelected.Disconnect(&mesh_box, &MeshVisualBox::EntitySelected);
}

void ComponentsView::Update(float sec) {
	Entity* entity = scene_view->GetSelectedEntity();
	if(entity) {
		for(Component* component : entity->GetComponents()) {
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SCALED(4.f), SCALED(4.f)));

			const bool open = ImGui::CollapsingHeader(component->GetName(), ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen);
			ImGui::SameLine(ImGui::GetWindowWidth() - SCALED(25.f));
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

				if(dynamic_cast<Mesh*>(component)) {
					mesh_box.Update();
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

void ComponentsView::ShowProperty(BaseProperty* baseProperty) const {
	if(dynamic_cast<Property<S32>*>(baseProperty)) {
		Property<S32>* prop = (Property<S32>*)baseProperty;
		ImGui::TextUnformatted(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::PushItemWidth(SCALED(70.f));
		int value = prop->Get();
		if(ImGui::DragInt("##" + prop->GetName(), &value)) {
			*prop = value;
		}
	} else if(dynamic_cast<Property<float>*>(baseProperty)) {
		Property<float>* prop = (Property<float>*)baseProperty;
		ImGui::TextUnformatted(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::PushItemWidth(SCALED(100.f));
		float value = prop->Get();
		if(ImGui::DragFloat("##" + prop->GetName(), &value)) {
			*prop = value;
		}
	} else if(dynamic_cast<Property<String>*>(baseProperty)) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(SCALED(6.f), SCALED(6.f)));

		const Property<String>* prop = (Property<String>*)baseProperty;
		ImGui::TextUnformatted(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "%s", prop->Get().ToCStr());

		ImGui::PopStyleVar();
	} else if(dynamic_cast<Property<Vector3D>*>(baseProperty)) {
		Property<Vector3D>* prop = (Property<Vector3D>*)baseProperty;
		ImGui::TextUnformatted(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::PushItemWidth(-5.f);
		Vector3D value = prop->Get();
		if(ImGui::DragFloat3("##" + prop->GetName(), value.GetData(), 0.1f)) {
			*prop = value;
		}
	} else if(dynamic_cast<Property<Quaternion>*>(baseProperty)) {
		Property<Quaternion>* prop = (Property<Quaternion>*)baseProperty;
		Vector3D axis = prop->Get().GetAxis();
		float angle = prop->Get().GetAngle();

		ImGui::TextUnformatted("Axis:");
		ImGui::SameLine(SCALED(100.f));
		if(ImGui::DragFloat3("##Axis" + prop->GetName(), axis.GetData(), 0.1f)) {
			*prop = Quaternion(axis, angle);
		}
		ImGui::TextUnformatted("Angle:");
		ImGui::SameLine(SCALED(100.f));
		if(ImGui::SliderFloat("##Angle" + prop->GetName(), &angle, 0.0f, 360.f)) {
			*prop = Quaternion(axis, angle);
		}
	} else if(dynamic_cast<Property<Color>*>(baseProperty)) {
		Property<Color>* prop = (Property<Color>*)baseProperty;
		ImGui::TextUnformatted(prop->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		Color value = prop->Get();
		if(ImGui::ColorEdit4(prop->GetName(), value.GetData(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
			*prop = value;
		}
	} else if(baseProperty->IsEnum()) {
		ImGui::TextUnformatted(baseProperty->GetName() + ":");
		ImGui::SameLine(SCALED(100.f));
		ImGui::TextUnformatted(baseProperty->ToString());
	} else {
	    const String errStr = String::Format("Can not draw property '#'", baseProperty->GetName());
		ImGui::TextColored(ImVec4(1.f, 0, 0, 1.f), "%s", errStr.ToCStr());
	}
}

void ComponentsView::ContextMenu(Entity* selectedEntity) const {
	if(ImGui::BeginPopupContextWindow("ComponentsOptions")) {
		if(ImGui::BeginMenu("Add Component", selectedEntity != nullptr)) {
			Factory<Component>* factory = game->GetComponentFactory();
			for(const String& componentName : factory->GetRegisteredComponentsName()) {
				ImGui::BeginDisabled(componentName == "Mesh");
				if(ImGui::MenuItem(componentName.ToCStr(), "", false)) {
					Component* newComponent = factory->Create(componentName);
					if(selectedEntity->GetComponent(typeid(*newComponent).hash_code())) {
						os->Messagebox("Error", "Component already exists");
						delete newComponent;
					} else {
						selectedEntity->AddComponent(newComponent);
					}
				}
				ImGui::EndDisabled();
			}

			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}
}
