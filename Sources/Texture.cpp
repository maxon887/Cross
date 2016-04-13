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
#include "Texture.h"

using namespace cross;

Texture::Texture(GLuint id, int width, int height) : 
	Texture(id, width, height, false)
{ }

Texture::Texture(GLuint id, int width, int height, bool generateMipmap) :
	id(id),
	width(width),
	height(height)
{
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	if(generateMipmap){
		SAFE(glGenerateMipmap(GL_TEXTURE_2D));
		ApplyFilter(Filter::TRILINEAR);
	}else{
		ApplyFilter(Filter::LINEAR);
	}
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture(){
	SAFE(glDeleteTextures(1, &id));
}

GLuint Texture::GetID(){
	return id;
}

int Texture::GetWidth(){
	return width;
}

int Texture::GetHeight(){
	return height;
}

void Texture::ApplyFilter(Filter filter){
	switch(filter){
	case cross::Texture::NEAREST:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		break;
	case cross::Texture::LINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::BILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::TRILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::NONE:
		break;
	default:
		throw CrossException("Wrong texture filter type");
	}
}