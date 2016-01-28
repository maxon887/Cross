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
#include "Debuger.h"
#include "Font.h"

SpritesScreen::~SpritesScreen(){
	delete awesome_face;
	delete debug_font;
}

void SpritesScreen::Start(){
	awesome_face = gfx2D->LoadImage("AwesomeFace.png", 0.1f);
	debug_font = new Font("LiberationMono-Bold.ttf", 50, Color::Red);
	/*
	for(int i = 0; i < SPRITES_COUNT; i++){
		positions[i].x = Random(5 + awesome_face->GetWidth() / 2, game->GetWidth() - 5 - awesome_face->GetWidth() / 2);
		positions[i].y = Random(5 + awesome_face->GetHeight() / 2, game->GetHeight() - 5 - awesome_face->GetHeight() / 2);

		velocities[i].x = Random(1.f, 5.f);
		velocities[i].y = Random(1.f, 5.f);
	}
	gfx2D->SetClearColor(Color(0.2f, 0.2f, 0.2f));*/
}

void SpritesScreen::Update(float sec){
	gfx2D->Clear();
	for(int i = 0; i < positions.size(); i++){
		positions[i] += velocities[i];
		float spriteWidth = awesome_face->GetWidth()/2;
		float spriteHeight = awesome_face->GetHeight()/2;

		if((positions[i].x - spriteWidth) <= 0 || (positions[i].x + spriteWidth) >= game->GetWidth()){
			velocities[i].x *= -1;
		}
		if((positions[i].y - spriteHeight) <= 0 || (positions[i].y + spriteHeight) >= game->GetHeight()){
			velocities[i].y *= -1;
		}
		gfx2D->DrawSprite(positions[i], awesome_face);
	}
	
	if(input->IsPressed(Key::ESCAPE)){
		game->SetScreen(new MainScreen());
	}
	PushSprite();
}

void SpritesScreen::PushSprite(){
	float fps = debuger->GetFPS();
	if(fps > 40.f){
		for(int i = 0; i < 20; i++){
			Vector2D position;
			position.x = Random(5 + awesome_face->GetWidth() / 2, game->GetWidth() - 5 - awesome_face->GetWidth() / 2);
			position.y = Random(5 + awesome_face->GetHeight() / 2, game->GetHeight() - 5 - awesome_face->GetHeight() / 2);
			positions.push_back(position);
			Vector2D velocity;
			velocity.x = Random(-3.f, 3.f);
			velocity.y = Random(-3.f, 3.f);
			velocities.push_back(velocity);
		}
	}else{
		string message = "Spris Drawn " + to_string(positions.size());
		gfx2D->DrawText(Vector2D(0.f, 10.f), message, debug_font);
	}
}