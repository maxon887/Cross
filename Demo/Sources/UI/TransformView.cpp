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
#include "TransformView.h"
#include "Entity.h"
#include "Transform.h"
#include "System.h"
#include "Demo.h"
#include "MenuBar.h"

#include "ThirdParty/ImGui/imgui.h"

void TransformView::WillContent() {
	ImGui::SetNextWindowSize(ImVec2((float)system->GetWindowWidth() / 3.f, 
							(float)(system->GetWindowHeight() - demo->GetMenuBar()->GetHeight()) / 3.f),
							ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() / 3.f * 2.f, demo->GetMenuBar()->GetHeight()), ImGuiCond_FirstUseEver);
}

void TransformView::Content(Transform* transform) {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(system->GetScreenScale(), system->GetScreenScale()));

	float posVec[3];
	memcpy(posVec, transform->GetPosition().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("Position", posVec, 0.1f)) {
		transform->SetPosition(Vector3D(posVec[0], posVec[1], posVec[2]));
	}

	float scaleVec[3];
	memcpy(scaleVec, transform->GetScale().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("Scale", scaleVec, 0.05f)) {
		transform->SetScale(Vector3D(scaleVec[0], scaleVec[1], scaleVec[2]));
	}

	Quaternion rotation = transform->GetRotate();
	float rotAxis[3];
	memcpy(rotAxis, rotation.GetAxis().GetData(), sizeof(Vector3D));
	if(ImGui::DragFloat3("Axis", rotAxis, 0.1f)) {
		transform->SetRotate(Vector3D(rotAxis[0], rotAxis[1], rotAxis[2]), rotation.GetAngle());
	}
	float angle = transform->GetRotate().GetAngle();
	if(ImGui::SliderFloat("Angle", &angle, 0.0f, 360.f)) {
		transform->SetRotate(rotation.GetAxis(), angle);
	}

	ImGui::PopStyleVar();
}
