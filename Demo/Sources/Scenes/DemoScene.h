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
#include "Utils/FreeCameraScene.h"
#include "System.h"

using namespace cross;

class DemoScene : public FreeCameraScene {
public:
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Update(float sec) override;

	virtual void Save(const String& file) override;

	void ApplyMaterial(Entity* entity, Material* mat, bool depthTest = true);
	void DrawVector(const Vector3D& vec, const Vector3D& pos = Vector3D::Zero);

private:
	const float camera_speed = 5.f;

	const String ArrowModelFile = "Models/Arrow.fbx";
	bool draw_vector = false;
	Entity* arrow = nullptr;
	Material* arrow_mat = nullptr;

	S32 handled_action		= -1;
	Vector2D touch_position = Vector2D();

	void OnActionDown(Input::Action action);
	void OnActionMove(Input::Action action);
	void OnActionUp(Input::Action action);
	void OnKeyPressed(Key key);
	void OnKeyReleased(Key key);

	void OnOrientationChanged(System::Orientation o);

	void MouseWheelRoll(float delta);
};
