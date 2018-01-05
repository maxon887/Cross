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
#include "Cross.h"

namespace cross {

/*	Abstract concept of a game Screen. Can be used for creating fundamental bases for 2D drawings, 3D scenes, splash screens etc.
	This is the basic structure managed by the Game class. It's react on all crucial game events, updated and utilized by Game.
	Screens can be set and obtained by Game::SetScreen and Game::GetCurrentScreen */
class Screen {
public:
	virtual ~Screen() { }
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
	/* Called when game need to be suspend like lost focus or input phone call */
	virtual void Suspend() { }
	/* Called when game about show again after suspending */
	virtual void Resume() { }

	/* Returns name of the Screen. Screens can be named to provide some uniquely behavior among same classes */
	const string& GetName() const;
	/* Sets name of this Screen. Can be set in a constructor or by loading from file for example */
	void SetName(const string& name);
	/* Set background color for areas than not covered any other stuff */
	void SetBackground(const Color& background);

private:
	string name						= "noname";
};
	
}
