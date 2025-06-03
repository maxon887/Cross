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
#include "Demo.h"

#include "ThirdParty/ImGui/imgui.h"

void TransformVisualBox::Update(Transform* transform) {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(SCALED(1.f), SCALED(1.f)));

	ImGui::NewLine();
	ImGui::SameLine(SCALED(15.f));
	ImGui::BeginGroup();
	float rawVec[3];

	ImGui::Text("Position:");
	ImGui::SameLine(SCALED(100.f));
	memcpy(rawVec, transform->GetPosition().GetData(), sizeof(Vector3D));
	ImGui::PushItemWidth(-5.f);
	if(ImGui::DragFloat3("##Position", rawVec, 0.1f)) {
		transform->SetPosition(Vector3D(rawVec[0], rawVec[1], rawVec[2]));
	}

	ImGui::Text("Scale:");
	ImGui::SameLine(SCALED(100.f));
	memcpy(rawVec, transform->GetScale().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("##Scale", rawVec, 0.05f)) {
		transform->SetScale(Vector3D(rawVec[0], rawVec[1], rawVec[2]));
	}

	Quaternion rotate = transform->GetRotate();

	ImGui::Text("Axis:");
	ImGui::SameLine(SCALED(100.f));
	memcpy(rawVec, rotate.GetAxis().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("##Axis", rawVec, 0.1f)) {
		transform->SetRotate(Vector3D(rawVec[0], rawVec[1], rawVec[2]), rotate.GetAngle());
	}
	ImGui::Text("Angle:");
	ImGui::SameLine(SCALED(100.f));
	float angle = rotate.GetAngle();
	if(ImGui::SliderFloat("##Angle", &angle, 0.0f, 360.f)) {
		transform->SetRotate(rotate.GetAxis(), angle);
	}

	ImGui::EndGroup();

	ImGui::PopItemWidth();

	ImGui::PopStyleVar(1);
}