/*	Copyright © 2018 Maksim Lukyanov

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

namespace cross{

/*	This class represents image data loaded to gpu.
	If you need to load Texture from file use Graphics2D class.
	Textures needed to create sprites and 3D models propertie maps */
class Texture {
public:
	enum Filter {
		NEAREST,
		LINEAR,
		BILINEAR,
		TRILINEAR
	};

	enum TilingMode {
		CLAMP_TO_EDGE,
		REPEAT
	};

	enum Compression {
		ETC1,
		NONE
	};

	static Byte* LoadRawData(const string& filename, int& width, int& height, int& channels);

	~Texture();

	void Load(const string& filename);
	void Load(const string& filename, Texture::TilingMode tillingMode);
	void Load(const string& filename, Texture::Filter filter);
	void Load(const string& filename, Texture::TilingMode tillingMode, Texture::Filter filter, bool compressed);
	void Save(const string& filename);
	void Create(U32 channels, U32 width, U32 height, Texture::Filter filter);
	void Create(Byte* data,
				U32 channels,
				U32 width,
				U32 height,
				Texture::Filter filter,
				Texture::Compression compression,
				Texture::TilingMode tilingMode,
				bool generateMipmaps);

	U32 GetWidth() const;
	U32 GetHeight() const;
	U32 GetChannels() const;
	U64 GetID() const;
	void SetName(const string& name);
	const string& GetName() const;
	void SetTilingMode(TilingMode mode);
	void SetFilter(Filter filter);
	void AddMipmapLevel(U32 level, U32 dataLen, Byte* data, U32 w, U32 h, Texture::Compression comp);
	Texture* Clone() const;

private:
	string name		= "noname";
	U64 id			= 0;
	U32 width		= 0;
	U32 height		= 0;
	U32 channels	= 0;

	void LoadRAW(const string& filename, Texture::Filter filter);
	void LoadPKM(const string& filename, Texture::Filter filter);
	void LoadKTX(const string& filename, Texture::Filter filter);
};

}
