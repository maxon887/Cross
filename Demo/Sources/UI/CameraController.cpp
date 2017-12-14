/*	Copyright © 2015 Lukyanau Maksim

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
#include "CameraController.h"

#include "ThirdParty/ImGui/imgui.h"

void CameraController::Content() {
	float value = 0.f;
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
	ImGui::VSliderFloat("", ImVec2(35.f, 180.f), &value, -1.f, 1.f, "%.2f");
	ImGui::PopStyleVar();

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const ImVec2 w = ImGui::GetWindowSize();
	float r = 160.0f;
	float x = p.x - 16.0f + w.x - r;
	float y = p.y - 40.0f + w.y - r - 180.f;
	float spacing = 8.0f;
	const ImU32 col32 = ImColor(1.f, 1.f, 0.33f);
	drawList->AddCircle(ImVec2(x + r*0.5f, y + r*0.5f), r*0.5f, col32, 30, 4);
}