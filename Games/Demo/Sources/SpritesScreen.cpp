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
#include "SpritesScreen.h"
#include "Graphics2D.h"
#include "Input.h"
#include "Game.h"
#include "MainScreen.h"
#include "Misc.h"
#include "Sprite.h"



void SpritesScreen::Start(){
	awesome_face = gfx2D->LoadImage("AwesomeFace.png", 0.1f);
	for(int i = 0; i < SPRITES_COUNT; i++){
		positions[i].x = Random(5 + awesome_face->GetWidth() / 2, game->GetWidth() - 5 - awesome_face->GetWidth() / 2);
		positions[i].y = Random(5 + awesome_face->GetHeight() / 2, game->GetHeight() - 5 - awesome_face->GetHeight() / 2);

		velocities[i].x = Random(1.f, 2.f);
		velocities[i].y = Random(1.f, 2.f);
	}
}

void SpritesScreen::Update(float sec){
	gfx2D->Clear();

	for(int i = 0; i < SPRITES_COUNT; i++){
		gfx2D->DrawSprite(positions[i], awesome_face);
	}
	
	if(input->IsPressed(Key::ESCAPE)){
		game->SetScreen(new MainScreen());
	}
}