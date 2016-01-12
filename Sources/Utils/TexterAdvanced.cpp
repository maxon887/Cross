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
#include "TexterAdvanced.h"
#include "Exception.h"

using namespace cross;

FT_Library TexterAdvanced::library = NULL;

TexterAdvanced::TexterAdvanced():
	face(NULL)
{

}

void TexterAdvanced::LoadFont(File* file){
	FT_Error error;
	if(library == NULL){
		error = FT_Init_FreeType(&library);
		if(error){
			throw CrossException("Error initializing FreeType library");
		}
	}
	//error = FT_New_Face(library, "times.ttf", 0, &face);
	error = FT_New_Memory_Face(library, file->data, file->size, 0, &face);
	if(error){
		throw CrossException("The font file could be opened and read, but it appears");
	}

	error = FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);
	if(error){
		throw CrossException("Error in set char size");
	}
	FT_UInt glyph_index = FT_Get_Char_Index(face, 0x41);
	error = FT_Load_Glyph(face, glyph_index, FT_RENDER_MODE_LCD_V);
	if(error){
		throw CrossException("Error in loading glyph");
	}


	delete file;
}

byte* TexterAdvanced::CharBitmap(int* width, int* height){
	FT_Error error;
	error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
	if(error){
		throw CrossException("Error rendering glyph");
	}
	//int top = face->glyph->bitmap_top;
	//int left = face->glyph->bitmap_left;
	//FT_Bitmap;
	*width = face->glyph->bitmap.width;
	*height = face->glyph->bitmap.rows;
	byte* data = face->glyph->bitmap.buffer;
	return data;
}