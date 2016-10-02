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

class Texture{
public:
	enum class Filter{
		NEAREST,
		LINEAR,
		BILINEAR,
		TRILINEAR,
		NONE
	};
	enum class TilingMode{
		CLAMP_TO_EDGE,
		REPEAT,
		NONE
	};
	enum class Compression{
		ETC1,
		NONE
	};

	Texture(GLuint id, int width, int height);
	Texture(GLuint id, int width, int height, Filter filter);
	~Texture();

	GLuint GetID() const;
	int GetWidth() const;
	int GetHeight() const;
	Texture* Clone() const;
	void SetName(const string& name);
	string GetName();
	void SetTilingMode(TilingMode mode);
	void AddMipmapLelel(int level, U32 dataLen, Byte* data, int w, int h, Texture::Compression comp);

private:
	string name;
	GLuint id;
	int width;
	int height;

	void ApplyFilter(Filter filter);
};

}