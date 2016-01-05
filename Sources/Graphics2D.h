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
#include "Cross.h"
#include "GraphicsGL.h"
#include "Matrix.h"
#include "VertexShader.h"
#include "File.h"
#include "Color.h"

#undef LoadImage;

namespace cross{

class Image;

class Graphics2D : public GraphicsGL{
public:
	Graphics2D();
	~Graphics2D();
	
	void Clear();
	void DrawImage(Vector2D pos, Image* img);
	/* Load Image from assert file */
	Image* LoadImage(string filename);
	Image* LoadImage(string filename, float scaleFactor);
	void ReleaseImage(Image* img);
private:
	VertexShader* vertex_shader;
	Shader* fragment_shader;
	Matrix projection;

	byte* LoadImageInternal(string filename, int* width, int* height);
};

}