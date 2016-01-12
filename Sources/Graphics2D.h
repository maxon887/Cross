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
class TexterShaders;
class TexterAdvanced;

class Graphics2D{
public:
	Graphics2D();
	~Graphics2D();
	
	void Clear();
	void DrawText(Vector2D pos, string text);
	void DrawImage(Vector2D pos, Image* img);
	Image* CreateImage(Image* src, Rect area, float scaleFactor);
	/* Load Image from assert file */
	Image* LoadImage(string filename);
	Image* LoadImage(string filename, float scaleFactor);
	Image* LoadImage(byte* data, int width, int height);
	void ReleaseImage(Image* img);
private:
	SpriteShaders* sprite_shaders;
	TexterShaders* texter_shaders;
	TexterAdvanced* texter;
	Matrix projection;

	byte* LoadImageInternal(string filename, int* width, int* height);
};

}