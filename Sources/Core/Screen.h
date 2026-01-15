#pragma once
#include "Cross.h"
#include "Base/String.h"

namespace cross {

/*	Abstract concept of a game Screen. Can be used for creating fundamental bases for 2D drawings, 3D scenes, splash screens etc.
	This is the basic structure managed by the Game class. It's react on all crucial game events, updated and utilized by Game.
	Screens can be set and obtained by Game::SetScreen and Game::GetCurrentScreen */
class Screen {
public:
	virtual ~Screen() = default;
	/* Called before screen show up */
	virtual void Start() { }
	/* Called when screen about to change */
	virtual void Stop() { }
	/* Called before regular Screen::Update() */
	virtual void PreUpdate(float sec) { }
	/* Called every frame update. Usually 60 times per second(60fps) */
	virtual void Update(float sec) { }
	/* Called after regular Screen::Update() */
	virtual void PostUpdate(float sec) { }
	/* Called when game need to be suspended when lost focus or input phone call */
	virtual void Suspend() { }
	/* Called when game about show again after suspending */
	virtual void Resume() { }

	/* Returns name of the Screen. Screens can be named to provide some uniquely behavior among same classes */
	const String& GetName() const;
	/* Sets name of this Screen. Can be set in a constructor or by loading from file for example */
	void SetName(const String& name);
	/* Set background color for areas than not covered any other stuff */
	void SetBackground(const Color& background);

private:
	String name						= "noname";
};
	
}
