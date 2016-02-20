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
#include "BlackScreen.h"
#include "Input.h"
#include "Game.h"
#include "Debugger.h"
#include "Input.h"

void BlackScreen::Start(){
	screen_debug = false;
	Debugger::Instance()->ScreenDebug(screen_debug);
	Debugger::Instance()->ConsoleDebug(!screen_debug);

	input->ActionDown += MakeDelegate(this, &BlackScreen::ActionDownHandler);
}

void BlackScreen::Stop(){
	input->ActionDown.RemoveDelegate(input->ActionDown.GetLastDelegate());
	Debugger::Instance()->ScreenDebug(true);
	Debugger::Instance()->ConsoleDebug(false);
}

void BlackScreen::Update(float sec){

	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)) {
		game->SetScreen(game->GetStartScreen());
	}
}

void BlackScreen::ActionDownHandler(Vector2D pos){
	screen_debug = !screen_debug;
	Debugger::Instance()->ScreenDebug(screen_debug);
	Debugger::Instance()->ConsoleDebug(!screen_debug);
}