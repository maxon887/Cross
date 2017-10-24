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
#include "Transformable.h"

namespace cross {

class PrimitiveDrawer;

/* Used for 2D drawing. Contain original texture sprite location and pivot */
class Sprite : public Transformable{
public:
	Sprite(Texture* texture, Rect region);
	Sprite(Texture* texture, Rect region, Vector2D pivot);
	~Sprite();
	/* Get width on texture occupied by sprite */
	float GetWidth() const;
	/* Get height on texture occupied by sprite */
	float GetHeight() const;
	/* Rotate sprite by angle in degrees clockwise */
	void SetRotate(float angle);
	/* You can override texture collor by this function */
	void SetColor(const Color& color);
	/* Gets overrided color(white by default) */
	Color GetColor() const;
	/* Make clone of this object */
	Sprite* Clone() const;

private:
	CROSS_FRIENDLY
	friend PrimitiveDrawer;

	static const GLushort indices[];
	static GLuint EBO;

	GLuint VBO			= 0;
	float width			= -1.f;
	float height		= -1.f;
	Color color			= Color::White;
	Texture* texture	= NULL;
	bool original		= true;

	Sprite(const Sprite& sprite);

	const GLushort* GetIndices() const;
	Texture* GetTexture();
};

}