#pragma once
#include "Cross.h"
#include "Base/Event.h"

namespace cross{

template<class BaseClass>
class Factory;

/*  Single Game class instance created at application launch and stays valid until application is closed.
    Based on it lifetime it is good place to store persistent data and logic here. Update virtual functions
    available for code that needs to be in sync with game loop.
    Game also responsible for Screens management and generic game events like Start()/Stop()/Suspend() etc. */
class Game {
public:
	/* Occurs when someone call SetScreen() */
	Event<Screen*> ScreenChanged;
	/* Occurs when game about to go background */
	Event<> Suspended;
	/* Occurs when game resumed from background */
	Event<> Resumed;

	Game();
	virtual ~Game();

	/* Called once when core modules initialized. Before first Screen()::Start() function. */
	virtual void Start();
	/* Called once before release all engine resources */
	virtual void Stop();
	/* Called before regular Game::Update() */
	virtual void PreUpdate(float sec) { }
	/* Called every frame update. This Update will be called in any game Screen */
	virtual void Update(float sec);
	/* Called after regular Game::Update() */
	virtual void PostUpdate(float sec) { }
	/* Called when game needs to be paused. For example input call or window lost focus */
	virtual void Suspend();
	/* Called when game needs to be restored from previous interrupt. See Suspend() */
	virtual void Resume();
	/* Will change current Screen at the end of current frame. Old Screen will be deleted */
	virtual void SetScreen(Screen* screen);

	/* Returns time in sec since game start */
	float GetRunTime() const;
	/* Returns active game Screen */
	Screen* GetCurrentScreen() const;
	/* Returns active game Scene if available */
	Scene* GetCurrentScene() const;
	/* Returns component factory for custom Components registration */
	Factory<Component>* GetComponentFactory() const;

	/* Engine specific */
	void EngineUpdate();
	bool IsSuspended() const;

protected:
	Factory<Component>* component_factory	= nullptr;
	Screen* current_screen					= nullptr;
	Screen* next_screen						= nullptr;
	U64 timestamp							= 0;
	U64 run_time							= 0;
	bool suspended							= false;

	void LoadNextScreen();
};

}
