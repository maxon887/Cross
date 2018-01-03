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
#include "Config.h"
#include "Utils/Debugger.h"
#include "File.h"

#include <stdlib.h>

#include "Libs/SOIL/SOIL.h"

using namespace cross;

struct PKM {
	Byte identifier[6];
	U16 dataType;
	U16 extendedWidth;
	U16 extendedHeight;
	U16 originalHeight;
	U16 originalWidth;
};

struct KTX {
	Byte identifier[12];
	U32 endianness;
	U32 glType;
	U32 glTypeSize;
	U32 glFormat;
	U32 glInternalFormat;
	U32 glBaseInternalFormat;
	U32 pixelWidth;
	U32 pixelHeight;
	U32 pixelDepth;
	U32 numberOfArrayElements;
	U32 numberOfFaces;
	U32 numberOfMipmapLevels;
	U32 bytesOfKeyValueData;
};

Byte* Texture::LoadRawData(const string& filename, int& width, int& height, int& channels) {
	File* textureFile = system->LoadAssetFile(filename);
	CROSS_RETURN(textureFile, NULL, "Can not load raw texture. File not found");
	Byte* image = SOIL_load_image_from_memory(textureFile->data, textureFile->size, &width, &height, &channels, SOIL_LOAD_AUTO);
	delete textureFile;
	CROSS_RETURN(image, NULL, "SOL can't convert file:\n Pay attention on image color channels");
	int newWidth = 1;
	int newHeight = 1;
	while(newWidth < width) {
		newWidth *= 2;
	}
	while(newHeight < height) {
		newHeight *= 2;
	}
	if(newWidth != width || newHeight != height) {
		CROSS_ASSERT(true, "Not power of 2 texture. Performance issue!");
		Byte* newImage = (Byte*)malloc(channels * newWidth * newHeight);
		for(int i = 0; i < height; i++) {
			memcpy(newImage + i * newWidth * channels, image + i * width * channels, width * channels);
			//Clamp to edge effect
			if(newWidth > width) {
				memcpy(newImage + i * newWidth * channels + width * channels, image + i * width * channels, channels);
			}
		}
		//Clamp to edge effect
		if(newHeight > height) {
			memcpy(newImage + height * channels * newWidth, image + (height - 1) * width * channels, width * channels);
		}
		width = newWidth;
		height = newHeight;
		image = newImage;
	}
	return image;
}

Texture::~Texture(){
	SAFE(glDeleteTextures(1, &id));
}

void Texture::Load(const string& filename) {
	Load(filename, config->GetTextureFilter());
}

void Texture::Load(const string& filename, Texture::TilingMode tillingMode) {
	Load(filename, tillingMode, config->GetTextureFilter(), config->UseCompressedTextures());
}

void Texture::Load(const string& filename, Texture::Filter filter) {
	Load(filename, Texture::TilingMode::CLAMP_TO_EDGE, filter, config->UseCompressedTextures());
}

void Texture::Load(const string& filename, Texture::TilingMode tillingMode, Texture::Filter filter, bool compressed) {
	name = filename;
	Debugger::Instance()->SetTimeCheck();
	if(!compressed) {
		LoadRAW(filename, filter);
	} else {
#if defined(WIN) || defined(IOS)
		LoadRAW(filename + ".png", filter);
#elif ANDROID
		LoadKTX(filename + ".ktx", filter);
#endif
	}
	SetTilingMode(tillingMode);
	float loadTime = Debugger::Instance()->GetTimeCheck();
	system->LogIt("Texture(%s) loaded in %0.1fms", filename.c_str(), loadTime);
}

void Texture::Save(const string& filename) {
#ifdef OPENGL
	File file;
	file.name = filename;
	file.size = GetWidth() * GetHeight() * GetChannels();
	file.data = new Byte[file.size * GetChannels()];
	SAFE(glBindTexture(GL_TEXTURE_2D, GetID()));
	SAFE(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, file.data));
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
	system->SaveFile(&file);
#else
	CROSS_ASSERT(false, "SaveTexture does not support by current graphics API");
#endif
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

GLuint Texture::GetID() const {
	return id;
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

void Texture::SetFilter(Filter filter) {
	switch(filter) {
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

void Texture::LoadRAW(const string& filename, Texture::Filter filter) {
	int width, height, channels;
	Byte* image = LoadRawData(filename, width, height, channels);
	CROSS_FAIL(image, "Texture can not be loaded. File not found");

	bool generateMipmap = filter == Texture::Filter::BILINEAR || filter == Texture::Filter::TRILINEAR;
	Create(image, channels, width, height, filter, Texture::Compression::NONE, Texture::TilingMode::CLAMP_TO_EDGE, generateMipmap);
}

void Texture::LoadPKM(const string& filename, Texture::Filter filter) {
	File* file = system->LoadAssetFile(filename);

	PKM pkm;
	U32 offset = sizeof(PKM);
	memcpy(&pkm, file->data, offset);

	Create(file->data + offset, 3, pkm.extendedWidth, pkm.extendedHeight, filter, Texture::Compression::ETC1, Texture::TilingMode::CLAMP_TO_EDGE, false);
}

void Texture::LoadKTX(const string& filename, Texture::Filter filter) {
	File* file = system->LoadAssetFile(filename);

	KTX ktx;
	U32 offset = sizeof(KTX);
	memcpy(&ktx, file->data, offset);

	U32 imageSize;
	memcpy(&imageSize, file->data + offset, sizeof(U32));
	offset += sizeof(U32);

	Byte* imageData = new Byte[imageSize];
	memcpy(imageData, file->data + offset, imageSize);
	offset += imageSize;

	Create(imageData, 3, ktx.pixelWidth, ktx.pixelHeight, filter, Texture::Compression::ETC1, Texture::TilingMode::CLAMP_TO_EDGE, false);

	U32 mipmapW = ktx.pixelWidth;
	U32 mipmapH = ktx.pixelHeight;
	for(U32 i = 1; i < ktx.numberOfMipmapLevels; i++) {
		U32 padding = 3 - ((imageSize + 3) % 4);

		CROSS_ASSERT(padding != 0, NULL, "Not zero padding");

		mipmapW /= 2;
		mipmapH /= 2;

		memcpy(&imageSize, file->data + offset, sizeof(U32));
		offset += sizeof(U32);
		memcpy(imageData, file->data + offset, imageSize);
		offset += imageSize;
		AddMipmapLevel(i, imageSize, imageData, mipmapW, mipmapH, Texture::Compression::ETC1);
	}
	delete[] imageData;
}

void Texture::Create(U32 channels, U32 width, U32 height, Texture::Filter filter) {
	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	switch(channels) {
	case 1:
		SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0));
		break;
	case 3:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0));
		break;
	case 4:
		SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));
		break;
	default:
		CROSS_ASSERT(false, NULL, "Can not create texture. Wrong texture channel count");
	}
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	this->id = id;
	this->width = width;
	this->height = height;
	this->channels = channels;
	SetFilter(filter);
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Create(	Byte* data,
						U32 channels,
						U32 width,
						U32 height,
						Texture::Filter filter,
						Texture::Compression compression,
						Texture::TilingMode tilingMode,
						bool generateMipmaps) {
	GLuint id;
	SAFE(glGenTextures(1, &id));
	SAFE(glBindTexture(GL_TEXTURE_2D, id));
	if(compression == Texture::Compression::NONE) {
		switch(channels) {
		case 1:
			SAFE(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data));
			break;
		case 3:
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
			break;
		case 4:
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			break;
		default:
			CROSS_ASSERT(false, NULL, "Can not create texture. Wrong texture channel count");
		}
	} else {
		switch(compression) {
		case cross::Texture::Compression::ETC1:
#ifdef ANDROID
		{
			uint32_t dataLength = ((width >> 2) * (height >> 2)) << 3;
			SAFE(glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, width, height, 0, dataLength, data));
		}break;
#else
			CROSS_ASSERT(false, NULL, "Can not create texture. ETC1 compression not supported by current platform");
#endif
		default:
			break;
		}
	}

	switch(tilingMode) {
	case cross::Texture::TilingMode::CLAMP_TO_EDGE:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		break;
	case cross::Texture::TilingMode::REPEAT:
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		break;
	default:
		break;
	}

	if(generateMipmaps) {
		SAFE(glGenerateMipmap(GL_TEXTURE_2D));
	}
	this->id = id;
	this->width = width;
	this->height = height;
	this->channels = channels;
	SetFilter(filter);
	SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}
