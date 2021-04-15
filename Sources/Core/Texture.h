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

/*	Texture is digital representation of the surface of an object. Like color, roughness etc.
	Texture class also can be used to draw 2D images. Like sprites, buttons etc.
	Texture life time managed by Scene. To load Texture you can use Scene::GetTexture(). Dublicated models won't be loaded */
class Texture {
public:
	/* Specify how pixel data will be gathered from image data */
	enum Filter {
		/* Nearest pixel. Means no filter will be applied */
		NEAREST,
		/* Linear interpolation will be applied for pixels */
		LINEAR,
		/* Mipmaps will be provided for distant objects and linear interpolation will be applied for pixels */
		BILINEAR,
		/* Mipmaps levels and pixels will be linearly interpolated */
		TRILINEAR
	};
	/* Specify how pixels will be picked up outside of 1, 1 uv coordinates */
	enum TilingMode {
		/* Similar to the edge pixels */
		CLAMP_TO_EDGE,
		/* Pixels will be mirrored */
		REPEAT
	};
	/* Specify Texture Compression */
	enum Compression {
		ETC1,
		NONE
	};
	/* Converts Filter enum to cross String */
	static String FilterToString(Filter filter);
	/* Converts readable String to Filter enum value */
	static Filter StringToFilter(const String& string);
	/* Loads raw texture data from file */
	static Byte* LoadRawData(const String& filename, int& width, int& height, int& channels);

	~Texture();

	/* Returns texture name of its file name */
	const String& GetName() const;
	/* Sets texture name */
	void SetName(const String& name);

	/* Returns Texture width */
	U32 GetWidth() const;
	/* Returns Texture height */
	U32 GetHeight() const;
	/* Returns Texture channels available */
	U32 GetChannels() const;
	/* Returns Texture internal ID */
	U64 GetID() const;

	/* Loads Texture from file */
	void Load(const String& filename);
	/* Loads Texture from file with provided tilling mode */
	void Load(const String& filename, Texture::TilingMode tillingMode);
	/* Loads Texture from file with provided filter */
	void Load(const String& filename, Texture::Filter filter);
	/* Loads Texture from file with provided filter, compression and tilling mode */
	void Load(const String& filename, Texture::TilingMode tillingMode, Texture::Filter filter, bool compressed);
	/* Save texture into a file */
	void Save(const String& filename);
	/* Creates empty Texture and allocates data for it */
	void Create(U32 channels, U32 width, U32 height, Texture::Filter filter);
	/* Creates Texture from raw data with provided attributes */
	void Create(Byte* data,
				U32 channels,
				U32 width,
				U32 height,
				Texture::Filter filter,
				Texture::Compression compression,
				Texture::TilingMode tilingMode,
				bool generateMipmaps);

	/* Sets Texture tilling mode */
	void SetTilingMode(TilingMode mode);
	/* Applies Texture filter. Better set this while loading */
	void SetFilter(Filter filter);
	/* Manually adds mipmap level to this texture. Used for compressed image loading */
	void AddMipmapLevel(U32 level, U32 dataLen, Byte* data, U32 w, U32 h, Texture::Compression comp);
	/* Copy Texture. Pixels data won't be copied while stored in GPU */
	Texture* Clone() const;

private:
	String name		= "noname";
	U64 id			= 0;
	U32 width		= 0;
	U32 height		= 0;
	U32 channels	= 0;
	bool mip_maps	= false;

	void LoadRAW(const String& filename, Texture::Filter filter);
	void LoadPKM(const String& filename, Texture::Filter filter);
	void LoadKTX(const String& filename, Texture::Filter filter);
};

}
