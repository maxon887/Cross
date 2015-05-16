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
	
#pragma once

#include "Game.h"
#include "Image.h"

#undef DrawText

/* Class responsible for drawing text in to screen */
class Texter{
public:
	Texter(Game* game);
	Texter(Game* game, const char* fontFilename,
			float width, float height,
			int columns, int rows,
			int asciiOffset, float scaleFactor);

	void DrawText(float x, float y, string text);
	void DrawText(PointX pos, string text);
	float GetWidth();
	float GetHeight();
	void SetScaleFactor(float scaleFactor);
private:
	void Init(Game* game, const char* fontFilename,
			float width, float height,
			int columns, int rows,
			int asciiOffset, float scaleFactor);
	Game* game;
	Graphics* graphics;
	Image* font;
	Image* letters[256];
	float width;
	float height;
	int offset;
	int count;
	float scale_factor;
};