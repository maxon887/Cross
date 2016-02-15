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

namespace cross{

class SpriteShaders;
class PrimitiveShaders;
class Camera;

/*	This class can be used for drawing 2D graphics.
	All coordinates supposed to be in Canvas space. */
class Graphics2D{
public:
	Font* GetDefaultFont();
	void SetCamera(Camera* camera);
	Camera* GetDefaultCamera();
	void DrawPoint(Vector2D pos, Color color);
	void DrawLine(Vector2D p1, Vector2D p2, Color color);
	void DrawRect(Rect rect, Color color);
	void DrawRect(Rect rect, Color color, bool filled);
	void DrawCircle(Vector2D center, float radius, Color color);
	void DrawCircle(Vector2D center, float radius, Color color, bool filled);
	void DrawCircle(Vector2D center, float radius, Color color, bool filled, int accuracy);
	int DrawText(Vector2D pos, string text);
	void DrawText(Vector2D pos, const string &text, Font* font);
	void DrawSprite(Sprite* sprite);
	void DrawSprite(Vector2D pos, Sprite* sprite);
	void DrawSprite(Sprite* sprite, Color color, bool monochrome);
	void DrawSprite(Sprite* sprite, Color color, Camera* cam, bool monochrome);
	Sprite* CreateImage(Sprite* src, Rect area, float scaleFactor);
	Sprite* LoadImage(string filename);
	Sprite* LoadImage(string filename, float scaleFactor);
	Sprite* LoadImage(CRByte* data, int bytesPerChannel, int width, int height);
	void ReleaseSprite(Sprite* img);
//Internal data. You don't need call any of this methods or modify variables
public:
	Graphics2D();
	~Graphics2D();
	void Update();
private:
	static const string def_font_filename;
	Font* current_font;
	Font* default_font;
	SpriteShaders* sprite_shaders;
	PrimitiveShaders* primitive_shaders;
	Camera* camera;
	Camera* default_camera;

	Camera* GetCamera();
};

}