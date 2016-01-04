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

using namespace cross;

static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

Image::Image(GLuint id, int texWidth, int texHeight, Rect region)
	:region(region){
	this->textureID = id;
	this->texWidth = texWidth;
	this->texHeight = texHeight;
	this->u1 = region.x / texWidth;
	this->v1 = region.y / texHeight;
	this->u2 = this->u1 + region.width / texWidth;
	this->v2 = this->v1 + region.height / texHeight;
	model = Matrix::CreateIdentity();
	translation = Matrix::CreateIdentity();
	scale = Matrix::CreateIdentity();
	rotation = Matrix::CreateIdentity();
	memset(vertices, 0, sizeof(float) * 16);

	vertices[0] = -region.width / 2.0f;
	vertices[1] = -region.height / 2.0f;
	vertices[2] = u1;
	vertices[3] = v2;

	vertices[4] = region.width / 2.0f;
	vertices[5] = -region.height / 2.0f;
	vertices[6] = u2;
	vertices[7] = v2;;

	vertices[8] = region.width / 2.0f;
	vertices[9] = region.height / 2.0f;
	vertices[10] = u2;
	vertices[11] = v1;

	vertices[12] = -region.width / 2.0f;
	vertices[13] = region.height / 2.0f;
	vertices[14] = u1;
	vertices[15] = v1;
}

void Image::SetPosition(Vector2D pos){
	model.SetTranslation(pos);
}

void Image::Scale(float factor){
	scale.m[0][0] = factor;
	scale.m[1][1] = factor;
	scale.m[2][2] = factor;
	model = rotation * scale * translation;
}

void Image::Rotate(float angle){
	rotation.SetRotationZ(angle);
	model = rotation * scale * translation;
}

GLuint Image::GetTextureID(){
	return textureID;
}

float Image::GetWidth(){
	return region.width;
}

float Image::GetHeight(){
	return region.height;
}

float* Image::GetVertices(){
	return vertices;
}

float* Image::GetModel(){
	return model.GetData();
}

GLushort* Image::GetIndices(){
	return indices;
}