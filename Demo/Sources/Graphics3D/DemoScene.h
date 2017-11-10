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
#include "Scene.h"
#include "Utils/FreeCameraScene.h"

using namespace cross;

class DemoScene : public FreeCameraScene {
public:
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Update(float sec) override;

	/* Handle input action down that not drop on UI elements */
	virtual void ActionDown(Input::Action action);
	/* Handle input action move that not drop on UI elements */
	virtual void ActionMove(Input::Action action);
	/* Handle input action up that not drop on UI elements */
	virtual void ActionUp(Input::Action action);

private:
	//used same concept as on the Input class
	List<pair<Input::Action, int> > action_stack;

	S32 handled_action		= -1;
	Vector2D touch_position = Vector2D();

	U32 action_down_del		= 0;
	U32 action_move_del		= 0;
	U32 action_up_del		= 0;

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	void OnEyeClick();
};
