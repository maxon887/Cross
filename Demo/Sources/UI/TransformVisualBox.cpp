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
#include "TransformVisualBox.h"
#include "Entity.h"
#include "Transform.h"
#include "System.h"
#include "Demo.h"

#include "ThirdParty/ImGui/imgui.h"

void TransformVisualBox::Show(Transform* transform) {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SCALED(1.f), SCALED(1.f)));

	ImGui::NewLine();
	ImGui::SameLine(SCALED(15.f));
	ImGui::BeginGroup();

	ImGui::Text("Position:");
	ImGui::SameLine(SCALED(100.f));
	float posVec[3];
	memcpy(posVec, transform->GetPosition().GetData(), sizeof(Vector3D));
	ImGui::PushItemWidth(-5.f);
	if(ImGui::DragFloat3("##Position", posVec, 0.1f)) {
		transform->SetPosition(Vector3D(posVec[0], posVec[1], posVec[2]));
	}

	ImGui::Text("Scale:");
	ImGui::SameLine(SCALED(100.f));
	float scaleVec[3];
	memcpy(scaleVec, transform->GetScale().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("##Scale", scaleVec, 0.05f)) {
		transform->SetScale(Vector3D(scaleVec[0], scaleVec[1], scaleVec[2]));
	}

	ImGui::Text("Axis:");
	ImGui::SameLine(SCALED(100.f));
	if(ImGui::DragFloat3("##Axis", axis.GetData(), 0.1f)) {
		transform->SetRotate(axis, angle);
	}
	ImGui::Text("Angle:");
	ImGui::SameLine(SCALED(100.f));
	if(ImGui::SliderFloat("##Angle", &angle, 0.0f, 360.f)) {
		transform->SetRotate(axis, angle);
	}

	ImGui::EndGroup();

	ImGui::PopItemWidth();

	ImGui::PopStyleVar(1);
}

void TransformVisualBox::EntitySelected(Entity* newEntity) {
	Transform* tr = newEntity->GetComponent<Transform>();
	axis = tr->GetRotate().GetAxis();
	angle = tr->GetRotate().GetAngle();
}