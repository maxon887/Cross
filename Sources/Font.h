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

namespace cross{

class Font{
public:
	static FT_Library library;
	FT_Face face;
	/* Font will be loaded from file. Available font formats
	can be found in FreeType library documentation. 
	Font size will be represented in physical pixels*/
	Font(string filename, int size, Color color);
private:
	Color color;
};

};