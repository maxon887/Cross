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
#include "Demo.h"
#include "Utils/Sprite.h"
#include "Utils/PrimitiveDrawer.h"

void SpritesScreen::Start(){
	Screen::Start();
	awesome_face = demo->GetCommonSprite("AwesomeFace.png");
	for(U32 i = 0; i < 20; i++) {
		Vector2D position;
		position.x = Random(5 + awesome_face->GetWidth() / 2, GetWidth() - 5 - awesome_face->GetWidth() / 2);
		position.y = Random(5 + awesome_face->GetHeight() / 2, GetHeight() - 5 - awesome_face->GetHeight() / 2);
		positions.push_back(position);
		Vector2D velocity;
		velocity.x = Random(-3.f, 3.f);
		velocity.y = Random(-3.f, 3.f);
		velocities.push_back(velocity);
	}
}

void SpritesScreen::Stop(){
	delete awesome_face;
	Screen::Stop();
}

void SpritesScreen::Update(float sec){
	Screen::Update(sec);
	for(U32 i = 0; i < positions.size(); i++){
		positions[i] += velocities[i] * sec * 100;
		float spriteWidth = awesome_face->GetWidth()/2;
		float spriteHeight = awesome_face->GetHeight()/2;

		if((positions[i].x - spriteWidth) <= 0 || (positions[i].x + spriteWidth) >= GetWidth()){
			velocities[i].x *= -1;
		}
		if((positions[i].y - spriteHeight) <= 0 || (positions[i].y + spriteHeight) >= GetHeight()){
			velocities[i].y *= -1;
		}
		PrimitiveDrawer::DrawSprite(positions[i], awesome_face);
	}
}