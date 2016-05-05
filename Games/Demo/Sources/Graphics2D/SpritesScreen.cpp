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
#include "Demo.h"
#include "Sprite.h"
#include "Utils/Debugger.h"
#include "Font.h"

void SpritesScreen::Start(){
	awesome_face = demo->GetCommonSprite("AwesomeFace.png");
	debug_font = new Font("Engine/Fonts/VeraMono-Bold.ttf", 50, Color::Red);
}

void SpritesScreen::Stop(){
	delete debug_font;
}

void SpritesScreen::Update(float sec){
	for(unsigned int i = 0; i < positions.size(); i++){
		positions[i] += velocities[i] * sec * 100;
		float spriteWidth = awesome_face->GetWidth()/2;
		float spriteHeight = awesome_face->GetHeight()/2;

		if((positions[i].x - spriteWidth) <= 0 || (positions[i].x + spriteWidth) >= GetWidth()){
			velocities[i].x *= -1;
		}
		if((positions[i].y - spriteHeight) <= 0 || (positions[i].y + spriteHeight) >= GetHeight()){
			velocities[i].y *= -1;
		}
		gfx2D->DrawSprite(positions[i], awesome_face);
	}
	PushSprite();
	
	if(input->IsPressed(Key::ESCAPE) || input->IsPressed(Key::BACK)){
		game->SetScreen(game->GetStartScreen());
	}
}

void SpritesScreen::PushSprite(){
	float fps = Debugger::Instance()->GetFPS();
	if(fps > 40.f){
		for(int i = 0; i < 20; i++){
			Vector2D position;
			position.x = Random(5 + awesome_face->GetWidth() / 2, GetWidth() - 5 - awesome_face->GetWidth() / 2);
			position.y = Random(5 + awesome_face->GetHeight() / 2, GetHeight() - 5 - awesome_face->GetHeight() / 2);
			positions.push_back(position);
			Vector2D velocity;
			velocity.x = Random(-3.f, 3.f);
			velocity.y = Random(-3.f, 3.f);
			velocities.push_back(velocity);
		}
	}else{
		string message = "Sprites Drawn " + to_string(positions.size());
		gfx2D->DrawText(Vector2D(0.f, 10.f), message, debug_font);
	}
}