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

namespace cross {

/* Used for 2D drawing. Contain original texture sprite location on it and pivot */
class Sprite{
public:
	Sprite(GLuint id, int textureWidth, int texureHeight, Rect region);
	Sprite(GLuint id, int textureWidth, int texureHeight, Rect region, Vector2D pivot);
	~Sprite();

	void SetPosition(const Vector2D& pos);
	void SetScale(float factor);
	void SetScale(const Vector2D& scale);
	void SetRotate(float angle);
	Vector2D GetPosition() const;
	GLuint GetTextureID() const;
	float GetWidth() const;
	float GetHeight() const;
	int GetTextureWidth() const;
	int GetTextureHeight() const;
	Sprite* Clone() const;
//Internal data. You don't need call any of this methods or modify variable
public:
	static GLuint EBO;

	GLuint VBO;

	const GLfloat* GetVertices() const;
	const GLushort* GetIndices() const;
	Matrix& GetModelMatrix();
private:
	static const GLushort indices[];
	Matrix model;
	Matrix scale;
	Matrix translate;
	Matrix rotation;
	bool recalc_model;
	GLuint textureID;
	GLfloat vertices[16];
	float width;
	float height;
	int texture_width;
	int texture_height;
};

}