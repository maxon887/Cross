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
#include "Cross.h"
#include "File.h"
#include "GraphicsGL.h"

#undef LoadImage
#undef DrawText

namespace cross{

class SpriteShaders;
class Font;

class Graphics2D{
public:
	Graphics2D();
	~Graphics2D();
	
	void Clear();
	void Clear(Color color);
	void DrawText(Vector2D pos, string text, Font* font);
	void DrawSprite(Vector2D pos, Sprite* sprite);
	void DrawSprite(Vector2D pos, Sprite* sprite, Color color, bool monochrome);
	Sprite* CreateImage(Sprite* src, Rect area, float scaleFactor);
	Sprite* LoadImage(string filename);
	Sprite* LoadImage(string filename, float scaleFactor);
	Sprite* LoadImage(byte* data, int bytesPerChannel, int width, int height);
	void ReleaseImage(Sprite* img);
private:
	SpriteShaders* sprite_shaders;
	Matrix projection;
	Color clear_color;
};

}