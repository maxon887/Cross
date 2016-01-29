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

#include "FreeType\ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#undef GetCharWidth
/*
struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;
struct FT_GlyphRec_;
typedef struct FT_GlyphRec_* FT_Glyph;*/

struct Glyph{
	unsigned int textureID;
	int bitmap_width;
	int bitmap_height;
	int bearingX;
	int bearingY;
	int advancedX;
};

namespace cross{

class Font{
public:
	FT_Face face;
	/* Font will be loaded from file. Available font formats
	can be found in FreeType library documentation. 
	Font size will be represented in virtual metrics*/
	Font(string filename, float size, Color color);
	~Font();
	Color GetColor();
	void SetColor(Color color);
	float GetSize();
	/* quite expencive function. 
	All bitmap char need to be racalculated. */
	float SetSize(float size);
	bool IsFixedWidth();
	float GetCharWidth();
	Glyph* GetGlyph(char c);
private:
	File* file;
	Glyph glyphs[128];
	FT_BitmapGlyph	bitmaps[128];
	unsigned int textures[128];
	Color color;
	float size;

	void Cache();
};

};