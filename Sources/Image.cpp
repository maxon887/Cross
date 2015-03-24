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
	
#include "Image.h"
#include "Graphics.h"

const short Image::indices[] = { 0, 1, 2,
								 0, 2, 3 };

Image::Image(GLuint id, int texWidth, int texHeight, RectX region)
	:region(region){
	this->textureID = id;
	this->region = region;
	this->u1 = region.x / texWidth;
	this->v1 = region.y / texHeight;
	this->u2 = this->u1 + region.width / texWidth;
	this->v2 = this->v1 + region.height / texHeight;
	angle = 0;
}

void Image::Scale(float factor){
	float width = region.width * factor;
	float height = region.height * factor;

	vertices[0] = -width/2.0f;
	vertices[1] = height/2.0f;
	vertices[2] = u1;
	vertices[3] = v2;
		
	vertices[4] = width/2.0f;
	vertices[5] = height/2.0f;
	vertices[6] = u2;
	vertices[7] = v2;;
		
	vertices[8] = width/2.0f;
	vertices[9] = -height/2.0f;
	vertices[10] = u2;
	vertices[11] = v1;
		
	vertices[12] = -width/2.0f;
	vertices[13] = -height/2.0f;
	vertices[14] = u1;
	vertices[15] = v1;
}

GLuint Image::GetTextureID(){
	return textureID;
}

int Image::GetWidth(){
	return region.width;
}

int Image::GetHeight(){
	return region.height;
}

float* Image::GetVertices(){
	return vertices;
}