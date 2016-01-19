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
#include "Font.h"
#include "Exception.h"
#include "Launcher.h"
#include "File.h"

using namespace cross;

FT_Library Font::library = NULL;

Font::Font(string filename, float size, Color color):
	color(color)
{
	File* fontFile = launcher->LoadFile(filename);
	FT_Error error;
	if(library == NULL){
		error = FT_Init_FreeType(&library);
		if(error){
			throw CrossException("Error initializing FreeType library");
		}
	}
	error = FT_New_Memory_Face(library, fontFile->data, fontFile->size, 0, &face);
	if(error){
		throw CrossException("The font file could be opened and read, but it appears");
	}

	SetSize(size);
}

Color Font::GetColor(){
	return color;
}

void Font::SetColor(Color color){
	this->color = color;
}

float Font::GetSize(){
	return size;
}

float Font::SetSize(float size){
	this->size = size;
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, size);
	if(error){
		throw CrossException("Error in set char size");
	}
}