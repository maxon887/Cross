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
//#include "GraphicsGL.h"
#include "Rect.h"
#include "Matrix.h"

namespace cross {

/* Class that reflected images loaded in to memory */
class Image{
public:
	/* Return virtual image width */
    float GetWidth();
	/* Return virtual image height */
    float GetHeight();

//Internal data. You don't need call any of this methods or modify variable
public:
	int texWidth;
	int texHeight;
	float angle;
    
	Image(unsigned int id, int texWidth, int texHeight, Rect region);
	void SetPosition(Vector2D pos);
	void Scale(float factor);
	void Rotate(float angle);
	unsigned int GetTextureID();
	float* GetVertices();
	float* GetModel();
	unsigned short* GetIndices();

private:
	Rect region;
	unsigned int textureID;
	Matrix model;
	Matrix scale;
	Matrix translation;
	Matrix rotation;
	float vertices[16];
	float u1, v1;
	float u2, v2;
};

}