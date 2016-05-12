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
#include "Graphics2D.h"

using namespace cross;

Texture::Texture(GLuint id, int width, int height) : 
	Texture(id, width, height, Filter::LINEAR)
{ }

Texture::Texture(GLuint id, int width, int height, Filter filter) :
	id(id),
	width(width),
	height(height)
{
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	ApplyFilter(filter);
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture(){
	//SAFE(glDeleteTextures(1, &id));
	gfx2D->ReleaseTexture(name, &id);
}

GLuint Texture::GetID() const{
	return id;
}

int Texture::GetWidth() const{
	return width;
}

int Texture::GetHeight() const{
	return height;
}

void Texture::SetName(const string& name){
	this->name = name;
}

string Texture::GetName(){
	return name;
}

Texture* Texture::Clone() const{
	return new Texture(*this);
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
		SAFE(glGenerateMipmap(GL_TEXTURE_2D));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::TRILINEAR:
		SAFE(glGenerateMipmap(GL_TEXTURE_2D));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::NONE:
		break;
	default:
		throw CrossException("Wrong texture filter type");
	}
}