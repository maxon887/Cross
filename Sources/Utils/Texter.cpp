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
#include "Graphics2D.h"
#include "Sprite.h"
#include "Game.h"

#undef DrawText

using namespace cross;

Texter::Texter(Game* game, const char* fontFilename,
				float width, float height,
				int columns, int rows,
				int asciiOffset){
	Init(game, fontFilename, width, height, columns, rows, asciiOffset, game->GetScaleFactor());
}

Texter::Texter(Game* game, const char* fontFilename,
				float width, float height,
				int columns, int rows,
				int asciiOffset, float scaleFactor){
	Init(game, fontFilename, width, height, columns, rows, asciiOffset, scaleFactor);
}

Texter::~Texter(){
	gfx2D->ReleaseImage(font);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			delete letters[i * columns + j];
		}
	}
}

void Texter::DrawText(Vector2D pos, string text){
	DrawText(pos.x, pos.y, text);
}

void Texter::DrawText(float x, float y, string text){
	x += GetWidth() / 2;
	y += GetHeight() / 2;
	for(unsigned int i = 0; i < text.length(); i++){
		gfx2D->DrawSprite(Vector2D(x + i * GetWidth(), y), letters[text[i] - offset]);
	}
}

float Texter::GetWidth(){
	return width / game->GetScaleFactor() * scale_factor;
}

float Texter::GetHeight(){
	return height / game->GetScaleFactor() * scale_factor;
}

void Texter::Init(	Game* game, const char* fontFilename,
					float width, float height,
					int columns, int rows,
					int asciiOffset, float scaleFactor){
	this->game = game;
	this->width = width;
	this->height = height;
	this->offset = asciiOffset;
	this->scale_factor = scaleFactor;
	this->count = 0;
	this->rows = rows;
	this->columns = columns;
	font = gfx2D->LoadImage(fontFilename);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			Rect reg(j * width, i * height, width, height);
			letters[i * columns + j] = gfx2D->CreateImage(font, reg, scaleFactor);
			count++;
		}
	}
}

void Texter::SetScaleFactor(float scaleFactor){
	for(int i = 0; i < count; i++){
		letters[i]->SetScale(scaleFactor);
	}
}
