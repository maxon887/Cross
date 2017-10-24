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
#include "GraphicsGL.h"

namespace cross{

/*	This class represents image data loaded to gpu.
	If you need to load Texture from file use Graphics2D class.
	Textures needed to create sprites and 3D models propertie maps */
class Texture{
public:
	enum Filter{
		NEAREST,
		LINEAR,
		BILINEAR,
		TRILINEAR
	};

	enum TilingMode{
		CLAMP_TO_EDGE,
		REPEAT
	};

	enum Compression{
		ETC1,
		NONE
	};

	Texture(GLuint id, U32 width, U32 height);
	Texture(GLuint id, U32 width, U32 height, U32 channels, Filter filter);
	~Texture();

	U32 GetWidth() const;
	U32 GetHeight() const;
	U32 GetChannels() const;
	GLuint GetID() const;
	void SetName(const string& name);
	const string& GetName() const;
	void SetTilingMode(TilingMode mode);
	void AddMipmapLevel(U32 level, U32 dataLen, Byte* data, U32 w, U32 h, Texture::Compression comp);
	Texture* Clone() const;

private:
	CROSS_FRIENDLY

	string name		= "noname";
	GLuint id		= 0;
	U32 width		= 0;
	U32 height		= 0;
	U32 channels	= 0;

	void ApplyFilter(Filter filter);
};

}
