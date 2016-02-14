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
#include "Launcher.h"
#include "File.h"
#include "GraphicsGL.h"

#include "FreeType\ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#undef GetCharWidth

using namespace cross;

static FT_Library library = NULL;

Font::Font(string filename, float size, Color color):
	color(color)
{
	file = launcher->LoadFile(filename);
	FT_Error error;
	if(library == NULL){
		error = FT_Init_FreeType(&library);
		if(error){
			throw CrossException("Error initializing FreeType library");
		}
	}
	error = FT_New_Memory_Face(library, file->data, file->size, 0, &face);
	if(error){
		throw CrossException("The font file could be opened and read, but it appears");
	}
	ZeroMemory(textures, sizeof(textures));
	SetSize(size);
}

Font::~Font(){
	delete file;
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

void Font::SetSize(float size){
	this->size = size;
	FT_Error error = FT_Set_Pixel_Sizes(face, 0, size);
	if(error){
		throw CrossException("Error in set char size");
	}
	Cache();
}

bool Font::IsFixedWidth(){
	return FT_IS_FIXED_WIDTH(face);
}

float Font::GetCharWidth(){
	if(IsFixedWidth()){
		FT_Error error = FT_Load_Char(face, 0x41, FT_LOAD_RENDER);
		if(error){
			throw CrossException("Can't load glyph");
		}
		return face->glyph->advance.x >> 6;
	}else{
		throw CrossException("Char width can be obtained only for monospace fonts");
	}
}

Glyph* Font::GetGlyph(char c){
	return &glyphs[c - 29];
}

void Font::Cache(){
	FT_Error error;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDeleteTextures(128, textures);
	glGenTextures(128, textures);
	for(int i = 0; i < 127; i++){
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		error = FT_Load_Glyph(face, i, FT_LOAD_RENDER);
		if(error){
			throw CrossException("Can't load glyph");
		}
		error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&bitmaps[i]);
		if(error){
			throw CrossException("Can't abtain glyph");
		}
		glyphs[i].textureID = textures[i];
		glyphs[i].bitmap_width = bitmaps[i]->bitmap.width;
		glyphs[i].bitmap_height = bitmaps[i]->bitmap.rows;
		glyphs[i].bearingX = face->glyph->metrics.horiBearingX >> 6;
		glyphs[i].bearingY = face->glyph->metrics.horiBearingY >> 6;
		glyphs[i].advancedX = face->glyph->advance.x >> 6;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, glyphs[i].bitmap_width, glyphs[i].bitmap_height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, bitmaps[i]->bitmap.buffer);
	}
}