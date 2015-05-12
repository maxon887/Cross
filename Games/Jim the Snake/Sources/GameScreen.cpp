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
	
#include "GameScreen.h"

GameScreen::GameScreen(Game* game):Screen(game){ }

void GameScreen::Start(){
	background = graphics->LoadRepeatedImage("Game/Background.jpg", game->GetWidth() + 50, game->GetHeight() + 50);
}

void GameScreen::Update(float sec){
	graphics->Clear(1,0,0);
	graphics->DrawImage(game->GetWidth() /2, game->GetHeight()/2, background);
}
