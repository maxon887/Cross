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
#include "CameraControllerView.h"
#include "Demo.h"
#include "System.h"
#include "Camera.h"
#include "Scene.h"
#include "Entity.h"
#include "CameraController.h"

#include "ThirdParty/ImGui/imgui.h"

CameraControllerView::CameraControllerView() : View("Camera")
{
	window_width = SCALED(170.f);
	window_height = SCALED(160.f);
}

void CameraControllerView::Shown() {
//	CameraController* cameraController = game->GetCurrentScene()->GetCamera()->GetEntity()->GetComponent<CameraController>();
//	if(cameraController) {
//		cameraController->LookAtCamera(false);
//	}
}

void CameraControllerView::PreUpdate() {
	ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
	ImGui::SetNextWindowPos(ImVec2((float)os->GetWindowWidth() - window_width, (float)os->GetWindowHeight() - window_height));

	SetFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
}

void CameraControllerView::Update(float sec) {
	if(!AvailableInMenu()) {
		Hide();
	}
	CameraController* cameraController = game->GetCurrentScene()->GetCamera()->GetEntity()->GetComponent<CameraController>();
	if(cameraController) {
		//bool lookAt = cameraController->IsLookAtCamera();
		bool lookAt = true;
		const ImVec2 cursor = ImGui::GetCursorScreenPos(); //must sit exactly there, before any drawings started

		float sliderValue = 0.f;
		ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, SCALED(25.f));
		if(ImGui::VSliderFloat("##", ImVec2(SCALED(25.f), SCALED(150.f)), &sliderValue, -1.f, 1.f, "")) {
			if(!lookAt) {
				cameraController->MoveUp(sliderValue * sec);
			} else {
				cameraController->MoveCloser(sliderValue * sec);
			}
		}

		ImGui::PopStyleVar();

		ImGui::SetCursorPos(ImVec2(SCALED(37.f), SCALED(5.f)));
		if(ImGui::Checkbox("Look At", &lookAt)) {
			//cameraController->LookAtCamera(lookAt);
			CROSS_ASSERT(false, "Unexpected behaviour");
		}

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImVec2 w = ImGui::GetWindowSize();
		float radius = SCALED(60.0f);
		Vector2D center(cursor.x - SCALED(16.0f) + w.x - radius, cursor.y - SCALED(16.0f) + w.y - radius);
		const ImU32 col32 = ImColor(1.f, 1.f, 0.33f);
		drawList->AddCircle(ImVec2(center.x, center.y), radius, col32, 30, SCALED(4.f));

		ImGuiIO &io = ImGui::GetIO();
		Vector2D mousePos = io.MousePos;
		Vector2D centerMouse = mousePos - center;

		if(io.MouseReleased[0]) {
			nav_pressed = false;
		}

		if(centerMouse.Length() < radius || nav_pressed) {
			if(centerMouse.Length() > radius) {
				centerMouse = centerMouse.GetNormalized() * radius;
			}
			if(io.MouseDown[0]) {
				nav_pressed = true;
				const ImU32 red32 = ImColor(1.f, 0.f, 0.0f);
				drawList->AddCircle(center + centerMouse, SCALED(6.f), red32, 12, SCALED(4.f));

				centerMouse /= radius;
				if(!lookAt) {
					cameraController->MoveForward(-centerMouse.y * sec);
					cameraController->MoveRight(centerMouse.x * sec);
				} else {
					cameraController->LookUp(-centerMouse.y * sec * 2.f);
					cameraController->MoveRight(-centerMouse.x * sec * 2.f);
				}
			}
		}

	}
}

void CameraControllerView::PostUpdate() {
	ImGui::PopStyleVar();
}

bool CameraControllerView::VisibleInMenu() {
	return os->IsMobile();
}

bool CameraControllerView::AvailableInMenu() {
	return game->GetCurrentScene() != nullptr;
}