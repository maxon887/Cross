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
	
#include "Texter.h"

Texter::Texter(Game* game, const char* fontFilename,
				float width, float height,
				int columns, int rows,
				int asciiOffset, float scaleFactor){
	this->game = game;
	this->graphics = game->graphics;
	this->width = width;
	this->height = height;
	this->offset = asciiOffset;
	this->scale_factor = scaleFactor;
	font = graphics->LoadImage(fontFilename);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			RectX reg(j * width, i * height, width, height);
			letters[i * columns + j] = graphics->CreateImage(font, reg, scaleFactor);
		}
	}
}

void Texter::DrawText(float x, float y, const char* text){
	x += GetWidth() / 2;
	y += GetHeight() / 2;
	int len = strlen(text);
	for(int i = 0; i < len; i++){
		graphics->DrawImage(x + i * GetWidth(), y, letters[text[i] - offset]);
	}
}

float Texter::GetWidth(){
	return width / game->GetScaleFactor() * scale_factor;
}

float Texter::GetHeight(){
	return height / game->GetScaleFactor() * scale_factor;
}