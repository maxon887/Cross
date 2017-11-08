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
#include "Demo.h"
#include "MainScreen.h"
#include "System.h"

Demo* demo = NULL;

void Demo::Start(){
	Game::Start();
	system->LogIt("Demo::Start()");
	demo = (Demo*)game;

	input->KeyPressed.Connect(this, &Demo::OnKeyPressed);
}

void Demo::Stop(){
	system->LogIt("Demo::Stop()");
	Game::Stop();
}

Screen* Demo::GetStartScreen(){
	return new MainScreen();
}

void Demo::OnBackClick(){
    SetScreen(GetStartScreen());
}

void Demo::OnKeyPressed(Key key){
	if(key == Key::ESCAPE || key == Key::BACK){
		SetScreen(GetStartScreen());
	}
}
