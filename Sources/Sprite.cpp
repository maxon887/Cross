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
#include "Sprite.h"

using namespace cross;

const GLushort Sprite::indices[] = { 0, 1, 2, 0, 2, 3 };

Sprite::Sprite(GLuint id, int texWidth, int texHeight, Rect region) :
	textureID(id),
	texture_width(texWidth),
	texture_height(texHeight),
	width(region.width),
	height(region.height)
{
	GLfloat u1 = region.x / texWidth;
	GLfloat v1 = region.y / texHeight;
	GLfloat u2 = u1 + region.width / texWidth;
	GLfloat v2 = v1 + region.height / texHeight;
	model = Matrix::CreateIdentity();
	st_mat = Matrix::CreateIdentity();
	rotation = Matrix::CreateIdentity();

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

Sprite::Sprite(GLuint id, int texWidth, int texHeight, Rect region, Vector2D pivot) :
	textureID(id),
	texture_width(texWidth),
	texture_height(texHeight),
	width(region.width),
	height(region.height)
{
	GLfloat u1 = region.x / texWidth;
	GLfloat v1 = region.y / texHeight;
	GLfloat u2 = u1 + region.width / texWidth;
	GLfloat v2 = v1 + region.height / texHeight;
	model = Matrix::CreateIdentity();
	rotation = Matrix::CreateIdentity();
	st_mat = Matrix::CreateIdentity();

	vertices[0] = -pivot.x;
	vertices[1] = -pivot.y;
	vertices[2] = u1;
	vertices[3] = v2;

	vertices[4] = -pivot.x + region.width;
	vertices[5] = -pivot.y;
	vertices[6] = u2;
	vertices[7] = v2;;

	vertices[8] = -pivot.x + region.width;
	vertices[9] = -pivot.y + region.height;
	vertices[10] = u2;
	vertices[11] = v1;

	vertices[12] = -pivot.x;
	vertices[13] = -pivot.y + region.height;
	vertices[14] = u1;
	vertices[15] = v1;
}

void Sprite::SetPosition(Vector2D pos){
	st_mat.SetTranslation(pos);
	model.SetTranslation(pos);
}

void Sprite::SetScale(float factor){
	st_mat.m[0][0] = factor;
	st_mat.m[1][1] = factor;
	model = rotation * st_mat;
}

void Sprite::SetScale(Vector2D scaleVec){
	st_mat.m[0][0] = scaleVec.x;
	st_mat.m[1][1] = scaleVec.y;
	model = rotation * st_mat;
}

void Sprite::SetRotate(float angle){
	rotation.SetRotationZ(angle);
	model = rotation * st_mat;
}

float* Sprite::GetModel(){
	return model.GetData();
}

const float* Sprite::GetVertices() const{
	return vertices;
}

const GLushort* Sprite::GetIndices() const{
	return (GLushort*)indices;
}

unsigned int Sprite::GetTextureID() const{
	return textureID;
}

float Sprite::GetWidth() const{
	return width * st_mat.m[0][0];
}

float Sprite::GetHeight() const{
	return height * st_mat.m[1][1];
}

int Sprite::GetTextureWidth() const{
	return texture_width;
}

int Sprite::GetTextureHeight() const{
	return texture_height;
}