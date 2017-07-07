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

namespace cross {

/*	Class needed for text drawing. Container for symbols and font params
	Available font formats can be found in FreeType library documentation. */
class Font{
public:
	Font(const Font& font);
	Font(string filename, float size, Color color);
	~Font();
	/* You can draw text with specific color(white by default) */
	void SetColor(Color color);
	/* Returns current font color */
	Color GetColor() const;
	/* Sets font size. Quite expencive function. All bitmap chars need to be racalculated. */
	void SetSize(float size);
	/* Returns virtual font size */
	float GetSize() const;
	/* Return true if current font is monospaced */
	bool IsFixedWidth() const;
	/* ONLY FOR MONOCHROM FONT! Return virtual char width. For regular font use GetCharWidthAdvance() */
	float GetCharWidth() const;
	/* Return certain char as Sprite from ASCII table */
	Sprite* GetChar(char c);
	/* Return virtual char width for specific symbol from ASCII */
	float GetCharWidthAdvance(char c) const;
	/* Make clone of this object */
	Font* Clone() const;

private:
	FT_Face face;
	File* file					= NULL;
	Array<Sprite*> sprites		= Array<Sprite*>(128, NULL);
	Array<Texture*> textures	= Array<Texture*>(128, NULL);
	Array<float> advances		= Array<float>(128, 0);
	Color color					= Color::White;
	float size					= 0;
	float char_width			= 0;
	bool original				= true;
	bool kill_textures			= false;

	void Cache();
	void KillTextures();
};

};