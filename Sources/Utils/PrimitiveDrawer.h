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
#include "Utils/Sprite.h"

namespace cross {

class PrimitiveDrawer {
public:
	//2D drawing
	static void DrawPoint(const Vector2D& pos, const Color& color);
	static void DrawLine(const Vector2D& p1, const Vector2D& p2, const Color& color);
	static void DrawRect(const Rect& rect, const Color& color);
	static void DrawRect(const Rect& rect, const Color& color, bool filled);
	static void DrawCircle(const Vector2D& center, float radius, const Color& color);
	static void DrawCircle(const Vector2D& center, float radius, const Color& color, bool filled);
	static void DrawCircle(const Vector2D& center, float radius, const Color& color, bool filled, U32 accuracy);
	//temporary sprites stuff
	static void DrawSprite(Sprite* sprite);
	static void DrawSprite(Vector2D pos, Sprite* sprite);
	static void DrawSprite(Sprite* sprite, Color color, bool monochrome);
	static void DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome);
	static void LoadSprites(Dictionary<string, Sprite*>& output, Texture* texture, string xml);
	//3d drawing
	static void DrawLine(const Vector3D& p1, const Vector3D& p2, const Color& c);
};
    
}
