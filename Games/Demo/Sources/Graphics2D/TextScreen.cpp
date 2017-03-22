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
#include "TextScreen.h"
#include "Graphics2D.h"
#include "Utils/Font.h"
#include "Input.h"
#include "Game.h"
#include "Internals/Debugger.h"

void TextScreen::Start(){
	debug_font = new Font("Engine/Fonts/VeraMonoBold.ttf", 50, Color::Red);
	font = new Font("Engine/Fonts/VeraMono.ttf", 50, Color::White);
}

void TextScreen::Stop(){
	for(ScreenChar* scrChar : chars){
		delete scrChar;
	}
	delete font;
	delete debug_font;
}

void TextScreen::Update(float sec){
	for(ScreenChar* scrChar : chars){
		font->SetColor(scrChar->color);
		gfx2D->DrawText(scrChar->positon, scrChar->str, font);
	}
	AddChar();
}

void TextScreen::AddChar(){
	float fps = Debugger::Instance()->GetFPS();
	if(fps > 40.f){
		for(U32 i = 0; i < 10; i++){
			float size = Random(10.f, 80.f);
			Vector2D position;
			position.x = Random(-size, GetWidth());
			position.y = Random(-size, GetHeight());
			Color color(Random(1.f), Random(1.f), Random(1.f));
			char c = Random(65, 122);
			string str(1, c);
			ScreenChar* screenChar = new ScreenChar();
			screenChar->positon = position;
			screenChar->color = color;
			screenChar->str = str;
			screenChar->size = size;
			chars.push_back(screenChar);
		}
	} else{
		string message = "Chars drawn " + to_string(chars.size());
		gfx2D->DrawText(Vector2D(0.f, 10.f), message, debug_font);
	}
}