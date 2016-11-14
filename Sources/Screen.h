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

namespace cross {

#define MAX_ACTIONS 20

/* Class reflect particular game scene. Like menu, level 1 etc.
   Every game must implement at least one Screen class.
   Witch will be loaded through Game::GetStartScreen function */
class Screen{
public:
	virtual ~Screen() { };
	/* Calls once before screen show up. */
	virtual void Start();
	/* Calls when screen about to change on new one */
	virtual void Stop();
	/* Calls every frame update. Ideally 60 times per second(60fps) */
	virtual void Update(float sec) { };
	virtual void LateUpdate(float sec);
	/* Calls where game need to be stoped like lost focus or input phone call */
	virtual void Suspend() { };
	/* Calls where game about show again after suspending */
	virtual void Resume() { };
	/* Return virtual game width for 2D drawing */
	virtual float GetWidth();
	/* Return virtual game height for 2D drawing */
	virtual float GetHeight();
	/* Handle input that not drop on UI elements */
	virtual void ActionDown(Input::Action action) { };
	virtual void ActionMove(Input::Action action) { };
	virtual void ActionUp(Input::Action action) { };

	bool IsScene();
	/* Reflect ratio between screen and target device width */
	float GetScaleFactor();
	/* Set background color for areas than not covered any other stuff */
	void SetBackground(const Color& background);
	/* New UI element will auto update and covers area from touches */
	void AddUI(UI* element);
protected:
	bool is_scene;
private:
	bool actionIDs[MAX_ACTIONS];
	CRArray<UI*> guis;

	FastDelegate1<Input::Action, void> action_down_delegate;
	FastDelegate1<Input::Action, void> action_move_delegate;
	FastDelegate1<Input::Action, void> action_up_delegate;

	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	bool OnGuiArea(Vector2D pos);
};
    
}
