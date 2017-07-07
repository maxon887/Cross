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
#include "Texture.h"

using namespace cross;

const GLushort Sprite::indices[] = { 0, 1, 2, 0, 2, 3 };
GLuint Sprite::EBO = -1;

Sprite::Sprite(Texture* texture, Rect region) :
	Transformable(),
	width(region.width),
	height(region.height),
	texture(texture)
{
	GLfloat u1 = region.x / texture->GetWidth();
	GLfloat v1 = region.y / texture->GetHeight();
	GLfloat u2 = u1 + region.width / texture->GetWidth();
	GLfloat v2 = v1 + region.height / texture->GetHeight();

	GLfloat vertices[16];
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

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(EBO == -1){
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

Sprite::Sprite(Texture* texture, Rect region, Vector2D pivot) :
	Transformable(),
	texture(texture),
	width(region.width),
	height(region.height)
{
	GLfloat u1 = region.x / texture->GetWidth();
	GLfloat v1 = region.y / texture->GetHeight();
	GLfloat u2 = u1 + region.width / texture->GetWidth();
	GLfloat v2 = v1 + region.height / texture->GetHeight();

	GLfloat vertices[16];
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

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if(EBO == -1) {
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

Sprite::Sprite(const Sprite& sprite) :
	Transformable(sprite),
	VBO(sprite.VBO),
	width(sprite.width),
	height(sprite.height),
	color(sprite.color),
	texture(sprite.texture),
	original(false)
{ }

Sprite::~Sprite(){
	if(original){
		glDeleteBuffers(1, &VBO);
	}
}

const GLushort* Sprite::GetIndices() const{
	return (GLushort*)indices;
}

Texture* Sprite::GetTexture(){
	return texture;
}

float Sprite::GetWidth() const{
	return width * scale.m[0][0];
}

float Sprite::GetHeight() const{
	return height * scale.m[1][1];
}

void Sprite::SetRotate(float angle){
	SetRotateZ(angle);
}

void Sprite::SetColor(const Color& color){
	this->color = color;
}

Color Sprite::GetColor() const{
	return color;
}

Sprite* Sprite::Clone() const{
	return new Sprite(*this);
}