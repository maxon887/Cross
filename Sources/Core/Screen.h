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
   Whitch will be loaded through Game::GetStartScreen() function */
class Screen{
public:
	/* Occurs when virtual screen space changed */
	Event<float, float> SizeChanged;

	virtual ~Screen() { };
	/* Called before screen show up. */
	virtual void Start();
	/* Called when screen about to change on new one */
	virtual void Stop();
	/* Called every frame update. Ideally 60 times per second(60fps) */
	virtual void Update(float sec);
	/* Called after regularly Screen::Update()  */
	virtual void LateUpdate(float sec);
	/* Called when game need to be suspend like lost focus or input phone call */
	virtual void Suspend() { };
	/* Called when game about show again after suspending */
	virtual void Resume() { };
	/* Return virtual game width for 2D drawing */
	virtual float GetWidth();
	/* Return virtual game height for 2D drawing */
	virtual float GetHeight();
	/* Handle input action down that not drop on UI elements */
	virtual void ActionDown(Input::Action action) { };
	/* Handle input action move that not drop on UI elements */
	virtual void ActionMove(Input::Action action) { };
	/* Handle input action up that not drop on UI elements */
	virtual void ActionUp(Input::Action action) { };
	/* Returns true if current screen is actually a 3D scene */
	bool IsScene();
	/* Reflect ratio between screen and target device width */
	float GetScaleFactor();
	/* Set background color for areas than not covered any other stuff */
	void SetBackground(const Color& background);
	/* New UI element will auto update and covers area from touches */
	void AddUI(UI* element);
	void EnableUI(bool enable);
	void EnableInputs(bool enable);

protected:
	bool is_scene;
	bool enable_inputs;

private:
	bool actionIDs[MAX_ACTIONS];
	Array<UI*> guis;

    U64 down_del;
    U64 up_del;
    U64 move_del;
	void ActionDownHandle(Input::Action action);
	void ActionMoveHandle(Input::Action action);
	void ActionUpHandle(Input::Action action);

	bool OnGuiArea(Vector2D pos);
};
    
}
