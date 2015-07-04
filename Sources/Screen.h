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

#include "Graphics.h"
#include "Input.h"
#include "Saver.h"
#include "Audio.h"

namespace cross {
    
class Game;

/* Class reflect particular game scene. Like menu, level 1 etc.
   Every game must implement at least one Screen class.
   Witch will be loaded through Game::GetStartScreen function */
class Screen{
public:
	/* Screen object need game class for internal purposes.
	   Do not delete screen yourself */
	Screen(Game* game);
	/* Calls once before screen show up. */
	virtual void Start();
	/* Calls every frame update. Ideally 60 times per second(60fps)*/
	virtual void Update(float sec);
	/* Calls where game need to be stoped like lost focus or input phone call */
	virtual void Suspend();
protected:
	Game* game;				//
	Launcher* launcher;		//
	Graphics* graphics;		// Copy of vital game modules just for comfortability
    Input* input;			//
	Saver* saver;			//
//Internal data. You don't need call any of this methods or modify variable
public:
	void Init();
	virtual ~Screen();
};
    
}
