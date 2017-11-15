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
#pragma once
#include "Cross.h"
#include "Input.h"
#include "Utils/FreeCameraScene.h"

using namespace cross;

class SceneView : public FreeCameraScene {
public:
	SceneView();

	void Start() override;
	void Stop() override;
	void Update(float sec) override;

	void OnEntitySelected(Entity* entity);
	void OnEntityGrabFocus(Entity* entity);

private:
	Vector2D pos;

	Shader* selection_shader		= NULL;
	Material* selection_material	= NULL;
	Entity* selected_entity			= NULL;

	void Draw(Entity* e);
	void EnableMesh(Entity* e, bool value);

	U64 action_down_del = 0;
	U64 action_move_del = 0;
	void OnActionDown(Input::Action action);
	void OnActionMove(Input::Action action);
};