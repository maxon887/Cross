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
#include "Event.h"

namespace cross{

/*	Core game class. Designed for contains function and fields shared between multiple Screens.
	Every game at least must override GetStartScreen() function
	in order to inform engine from which Screen it should start */
class Game {
public:
	/* Occurs when someone call SetScreen() */
	Event<Screen*> ScreenChanged;
	Event<Scene*> SceneLoaded;

	/* Returns Screen from what game need to be start */
	virtual Screen* GetStartScreen() = 0;

	Game();
	virtual ~Game();
	/* Called when core modules initialized. Before first Screen()::Start() function. */
	virtual void Start();
	/* Called before release all engine resources */
	virtual void Stop();
	virtual void PreUpdate(float sec) { }
	virtual void Update(float sec) { }
	virtual void PostUpdate(float sec) { }
	/* Called when game needs to be paused. For example input call or window lost focus */
	virtual void Suspend();
	/* Called when game needs to be restored from previous interrupt. See Suspend() */
	virtual void Resume();
    /* Will change current Screen at the end of frame. Old Screen will be deleted */
    virtual void SetScreen(Screen* screen);
	virtual void SetScene(Scene* scene);
	virtual void SetScene(Scene* scene, const string& filename);

	/* Returns time in sec since game start */
	float GetRunTime() const;
	/* Returns active game Screen */
	Screen* GetCurrentScreen();
	/* Returns active game Scene if available */
	Scene* GetCurrentScene();
	/* Exit from application */
    void Exit();
	/* Engine specific */
	void EngineUpdate();
	bool IsSuspended() const;

protected:
	Screen* current_screen	= NULL;
	Screen* next_screen		= NULL;
	string scene_file		= "";
	U64 timestamp			= 0;
	U64 run_time			= 0;
	bool suspended			= false;

	void LoadNextScreen();
};
    
}
