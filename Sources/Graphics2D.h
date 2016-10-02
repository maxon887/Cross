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
#include "Texture.h"

namespace cross{

/*	This class can be used for drawing 2D graphics. If you not support Camera2D all drawings
	will be placed in device screen metrics. */
class Graphics2D{
public:
	void SetCamera(Camera2D* camera);
	Camera2D* GetCamera();
	Camera2D* GetDefaultCamera();

	void DrawPoint(Vector2D pos, Color color);
	void DrawLine(Vector2D p1, Vector2D p2, Color color);
	void DrawRect(Rect rect, Color color);
	void DrawRect(Rect rect, Color color, bool filled);
	void DrawCircle(Vector2D center, float radius, Color color);
	void DrawCircle(Vector2D center, float radius, Color color, bool filled);
	void DrawCircle(Vector2D center, float radius, Color color, bool filled, int accuracy);
	void DrawText(Vector2D pos, string text);
	void DrawText(Vector2D pos, const string &text, Font* font);
	void DrawSprite(Sprite* sprite);
	void DrawSprite(Vector2D pos, Sprite* sprite);
	void DrawSprite(Sprite* sprite, Color color, bool monochrome);
	void DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome);

	Texture* LoadTexture(const string& filename);
	Texture* LoadTexture(const string& filename, Texture::Filter filter);
	Texture* LoadTexture(const string& filename, Texture::Filter filter, bool compressed);
	void ReleaseTexture(const string& filename, GLuint* id);
	Texture* CreateTexture(int channels, int width, int height, Texture::Filter filter);
	Texture* CreateTexture(	Byte* data,
							int channels,
							int width,
							int height,
							Texture::Filter filter,
							Texture::Compression compression,
							Texture::TilingMode tilingMode,
							bool generateMipmaps);
	void LoadSprites(CRDictionary<string, Sprite*>& output, Texture* texture, string xml);
	Font* GetDefaultFont();
//Internal data. You don't need call any of this methods or modify variables
public:
	Graphics2D();
	~Graphics2D();
	void Update(float sec);
private:
	Shader* texture_shader;
	Shader* monochrome_shader;
	Shader* simple_shader;
	Font* default_font;
	Camera2D* camera;
	Camera2D* default_camera;
	CRList<pair<Texture*, int>> loaded_textures;

	Texture* LoadRAWTexture(const string& filename, Texture::Filter filter);
	Texture* LoadPKMTexture(const string& filename, Texture::Filter filter);
	Texture* LoadKTXTexture(const string& filename, Texture::Filter filter);
};

}