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

#undef GetCharWidth

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;
struct FT_BitmapGlyphRec_;
typedef struct FT_BitmapGlyphRec_* FT_BitmapGlyph;

namespace cross {

struct Glyph{
	unsigned int textureID;
	int bitmap_width;
	int bitmap_height;
	int bearingX;
	int bearingY;
	int advancedX;
};

/*	Vital class for rendring texts. Available font formats
	can be found in FreeType library documentation.
	Font size will be represented in virtual canvas metrics. */
class Font{
public:
	Font(string filename, float size, Color color);
	~Font();

	Color GetColor();
	void SetColor(Color color);
	float GetSize();
	/* Quite expencive function. All bitmap chars need to be racalculated. */
	void SetSize(float size);
	bool IsFixedWidth();
	float GetCharWidth();
	Glyph* GetGlyph(char c);
private:
	FT_Face face;
	File* file;
	Glyph glyphs[128];
	FT_BitmapGlyph	bitmaps[128];
	unsigned int textures[128];
	Color color;
	float size;

	void Cache();
};

};