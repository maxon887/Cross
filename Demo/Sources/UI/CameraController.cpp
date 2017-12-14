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
#include "Game.h"
#include "System.h"
#include "Camera.h"
#include "Utils/FreeCameraScene.h"

#include "ThirdParty/ImGui/imgui.h"

static float window_width = 225.f;
static float window_height = 200.f;

void CameraController::WillContent() {
	ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
	ImGui::SetNextWindowPos(ImVec2(system->GetWindowWidth() - window_width, system->GetWindowHeight() - window_height));

	SetWindowFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
}

void CameraController::Content(float sec) {
	const ImVec2 cursor = ImGui::GetCursorScreenPos();

	float value = 0.f;
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
	ImGui::VSliderFloat("", ImVec2(35.f, 190.f), &value, -1.f, 1.f, "%.2f");
	ImGui::PopStyleVar();

	FreeCameraScene* scene = dynamic_cast<FreeCameraScene*>(game->GetCurrentScene());
	if(scene) {
		bool lookAt = scene->IsLookAtCamera();
		ImGui::SetCursorPos(ImVec2(47.f, 5.f));
		if(ImGui::Checkbox("Look At", &lookAt)) {
			scene->LookAtCamera(lookAt);
		}
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const ImVec2 w = ImGui::GetWindowSize();
	float radius = 80.0f;
	float x = cursor.x - 16.0f + w.x - radius;
	float y = cursor.y - 16.0f + w.y - radius;
	float spacing = 8.0f;
	const ImU32 col32 = ImColor(1.f, 1.f, 0.33f);
	drawList->AddCircle(ImVec2(x, y), radius, col32, 30, 4);

	ImGuiIO &io = ImGui::GetIO();
	Vector2D toRadius = Vector2D(x, y) - Vector2D(io.MousePos.x, io.MousePos.y);
	if(toRadius.Length() < radius) {
		if(io.MouseDown[0]) {
			const ImU32 red32 = ImColor(1.f, 0.f, 0.0f);
			drawList->AddCircle(ImVec2(io.MousePos.x, io.MousePos.y), 6, red32, 12, 4);

			toRadius.x /= radius;
			toRadius.y /= radius;

			scene->MoveForward(toRadius.y * sec);
			scene->MoveLeft(toRadius.x * sec);
		}
	}
}