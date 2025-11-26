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
#include "UI/MenuBar.h"

#include "ThirdParty/ImGui/imgui.h"

CameraControllerView::CameraControllerView() : View("Camera")
{ }

void CameraControllerView::PreUpdate() {
	if(os->IsMobile()) {
		if(os->GetDeviceOrientation() == System::Orientation::LANDSCAPE) {
			ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth() / 3.f,
											((float)os->GetWindowHeight() - demo->GetMenuBar()->GetHeight())),
									 ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight()), ImGuiCond_FirstUseEver);
		} else {
			ImGui::PushFont(demo->big_font);
			float halfHeight = ((float)os->GetWindowHeight() - demo->GetMenuBar()->GetHeight()) / 2.f;
			ImGui::SetNextWindowSize(ImVec2((float)os->GetWindowWidth(), halfHeight));
			ImGui::SetNextWindowPos(ImVec2(0, demo->GetMenuBar()->GetHeight() + halfHeight));
		}
	}
}

void CameraControllerView::Update(float sec) {
	if(!AvailableInMenu()) {
		Hide();
		return;
	}

	CameraController* cameraController = game->GetCurrentScene()->GetCamera()->GetEntity()->GetComponent<CameraController>();

	// Mode Selector
	const char* cameraModes[] = { "Orbit", "Free", "Pad" };
	int selectedModeIndex = cameraController->GetCameraMode();
	const char* previewMode = cameraModes[selectedModeIndex];

	if (ImGui::BeginCombo("Mode", previewMode)) {
		for (int n = 0; n < IM_ARRAYSIZE(cameraModes); n++)
		{
			const bool is_selected = (selectedModeIndex == n);
			if (ImGui::Selectable(cameraModes[n], is_selected)) {
				// new camera mode selected
				selectedModeIndex = n;
				cameraController->SetCameraMode((CameraController::Mode)selectedModeIndex);
			}


			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	// Height Control
	float sliderValue = 0.f;
	ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, SCALED(25.f));
	if(ImGui::VSliderFloat("##", ImVec2(SCALED(25.f), SCALED(150.f)), &sliderValue, -1.f, 1.f, "")) {
		cameraController->MoveCloser(sliderValue * sec);
	}
	ImGui::PopStyleVar();
	ImGui::SameLine();

	// Position Control
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	const ImVec2 windowSize = ImGui::GetContentRegionAvail();
	float padding = SCALED(20.f);
	float radius = windowSize.y / 2.f - padding;
	const ImVec2 cursor = ImGui::GetCursorScreenPos();
	Vector2D center(windowSize.x - radius - padding + cursor.x, radius + padding + cursor.y);
	const ImU32 col32 = ImColor(1.f, 1.f, 0.33f);
	drawList->AddCircle(ImVec2(center.x, center.y), radius, col32, 30, SCALED(4.f));

	ImGuiIO &io = ImGui::GetIO();
	Vector2D mousePos = io.MousePos;
	Vector2D centerMouse = mousePos - center;
	os->LogIt("# #", centerMouse.x, centerMouse.y);

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

			cameraController->MoveForward(-centerMouse.y * sec);
			cameraController->MoveRight(centerMouse.x * sec);
		}
	}
}

void CameraControllerView::PostUpdate() {
	if(os->IsMobile()) {
		if(os->GetDeviceOrientation() != System::Orientation::LANDSCAPE) {
			ImGui::PopFont();
		}
	}
}

bool CameraControllerView::VisibleInMenu() {
	return os->IsMobile();
}

bool CameraControllerView::AvailableInMenu() {
	Scene* scene = game->GetCurrentScene();
	if(!scene) {
		return false;
	}
	Camera* camera = scene->GetCamera();
	if(!camera) {
		return false;
	}
	CameraController* cameraController = camera->GetEntity()->GetComponent<CameraController>();
	if(!cameraController) {
		return false;
	}
	return true;
}