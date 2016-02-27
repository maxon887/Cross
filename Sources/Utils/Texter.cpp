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
				int asciiOffset)
{
	Init(game, fontFilename, width, height, columns, rows, asciiOffset, 1.0);
}

Texter::Texter(Game* game, const char* fontFilename,
				float width, float height,
				int columns, int rows,
				int asciiOffset, float scaleFactor)
{
	Init(game, fontFilename, width, height, columns, rows, asciiOffset, scaleFactor);
}

Texter::~Texter(){
	gfx2D->ReleaseSprite(font);
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			delete letters[i * columns + j];
		}
	}
}

void Texter::DrawText(Vector2D pos, const string &text){
	DrawText(pos.x, pos.y, text);
}

void Texter::DrawText(float x, float y, const string &text){
	x += GetWidth() / 2;
	y += GetHeight() / 2;
	for(unsigned int i = 0; i < text.length(); i++){
		letters[text[i] - offset]->SetPosition(Vector2D(x + i * GetWidth(), y));
		gfx2D->DrawSprite(letters[text[i] - offset], Color::White, gfx2D->GetDefaultCamera(), false);
	}
}

float Texter::GetWidth(){
	return width /  scale_factor;
}

float Texter::GetHeight(){
	return height / scale_factor;
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
