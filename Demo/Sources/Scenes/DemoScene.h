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
#pragma once
#include "Scene.h"
#include "System.h"

using namespace cross;

class CameraController;

class DemoScene : public Scene {
public:
	//Service entities will not be shown on SceneView and will be removed before scene saving
	Entity* service_root = nullptr;

	void Start() override;
	void Stop() override;
	void Update(float sec) override;

	void Save(const String& file) override;

	void ApplyMaterial(Entity* entity, Material* mat, bool depthTest = true);
	void ImportModel(const String& modelFile, bool calcTangents = false);
	void DrawVector(const Vector3D& vec, const Vector3D& pos = Vector3D::Zero);

protected:
	CameraController* camera_controller = nullptr;

private:
	const String ArrowModelFile = "Models/Arrow.fbx";

	bool draw_vector = false;
	Entity* arrow = nullptr;
	Material* arrow_mat = nullptr;

	void OnOrientationChanged(System::Orientation o);
};
