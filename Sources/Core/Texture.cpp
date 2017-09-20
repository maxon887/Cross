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
#include "System.h"
#include "Graphics2D.h"

using namespace cross;

Texture::Texture(GLuint id, U32 width, U32 height) : 
	Texture(id, width, height, 4, Filter::LINEAR)
{ }

Texture::Texture(GLuint id, U32 width, U32 height, U32 channels, Filter filter) :
	id(id),
	width(width),
	height(height),
	channels(channels)
{
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	ApplyFilter(filter);
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture(){
	SAFE(glDeleteTextures(1, &id));
}

GLuint Texture::GetID() const{
	return id;
}

U32 Texture::GetWidth() const{
	return width;
}

U32 Texture::GetHeight() const{
	return height;
}

U32 Texture::GetChannels() const{
	return channels;
}

void Texture::SetName(const string& name){
	this->name = name;
}

const string& Texture::GetName() const{
	return name;
}

void Texture::SetTilingMode(Texture::TilingMode mode){
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(mode) {
	case cross::Texture::TilingMode::CLAMP_TO_EDGE:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		break;
	case cross::Texture::TilingMode::REPEAT:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		break;
	default:
		CROSS_ASSERT(false, "Unknown tiling mode");
	}
}

void Texture::AddMipmapLevel(U32 level, U32 dataLen, Byte* data, U32 w, U32 h, Texture::Compression comp){
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(comp) {
	case cross::Texture::Compression::ETC1:
#ifdef ANDROID
		SAFE(glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_ETC1_RGB8_OES, w, h, 0, dataLen, data));
#endif // ANDROID
		break;
	case cross::Texture::Compression::NONE:
		SAFE(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		break;
	default:
		CROSS_ASSERT(false, "Unsupported compression format");
	}

	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture* Texture::Clone() const{
	return new Texture(*this);
}

void Texture::ApplyFilter(Filter filter){
	switch(filter){
	case cross::Texture::Filter::NEAREST:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		break;
	case cross::Texture::Filter::LINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::Filter::BILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	case cross::Texture::Filter::TRILINEAR:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		break;
	default:
		CROSS_ASSERT(false, "Wrong texture filter type");
	}
}
