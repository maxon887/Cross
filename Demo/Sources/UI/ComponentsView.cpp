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

#include "ThirdParty/ImGui/imgui.h"

ComponentsView::ComponentsView(SceneView* sceneView) :
	View("Components")
{
	scene_view = sceneView;
	scene_view->EntitySelected.Connect(&tranform_box, &TransformVisualBox::EntitySelected);
}

void ComponentsView::Content(float sec) {
	Entity* entity = scene_view->GetSelectedEntity();
	if(entity) {
		for(Component* component : entity->GetComponents()) {
			if(ImGui::CollapsingHeader(component->GetName())) {
				if(dynamic_cast<Transform*>(component)) {
					tranform_box.Show((Transform*)component);
				} else {
					for(BaseProperty* prop : component->GetProperties()) {
						ShowProperty(prop);
					}
				}
			}
		}
	}
}

void ComponentsView::ShowProperty(BaseProperty* baseProperty) {
	if(dynamic_cast<Property<S32>*>(baseProperty)) {
		Property<S32>* prop = (Property<S32>*)baseProperty;
		ImGui::DragInt(prop->GetName(), &prop->value);
	} else if(dynamic_cast<Property<float>*>(baseProperty)) {
		Property<float>* prop = (Property<float>*)baseProperty;
		ImGui::DragFloat(prop->GetName(), &prop->value);
	} else if(dynamic_cast<Property<String>*>(baseProperty)) {
		Property<String>* prop = (Property<String>*)baseProperty;
		ImGui::InputText(prop->GetName(), prop->value, prop->value.Length());
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