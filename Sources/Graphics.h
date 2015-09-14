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

#include "Color.h"
#include "Point.h"
#include "Image.h"

namespace cross {

#ifdef WIN
    #define glOrthof glOrtho
    #define GL_CLAMP_TO_EDGE GL_CLAMP
    #undef LoadImage
#endif

class Game;

/* Class responsible for drawing objects in the sceen */
class Graphics{
public:
	/*	Clear screen with particular color.*/
	void Clear(Color c);
	void Clear(float r, float g, float b);
	void SetViewPort(Point c, float width, float height);
	/* Scale Image */
	void ScaleImage(Image* img, float factor);
	/* Rotate Image counterclockwise in angle degree */
	void Rotate(Image* img, float angle);
	/* Create Image from source. Data do not copy. */
	Image* CreateImage(Image* src, Rect region);
	/* Create Image from source with sacale factor. Data do not copy. */
	Image* CreateImage(Image* src, Rect region, float scaleFactor);
	/* Load Image from file */
	Image* LoadImage(string filename);
	/* Load scaled image from file */
	Image* LoadImage(string filename, float scaleFactor);
	/* Delete all data related with Image */
	void ReleaseImage(Image* img);
	/* Load image filled with images */
	Image* LoadRepeatedImage(string filename, float width, float height);
	/* Load scaled image filled with images */
	Image* LoadRepeatedImage(string filename, float width, float height, float scaleFactor);
	void DrawPixel(Point p, Color c);
	void DrawPixel(Point p, float r, float g, float b);
	void DrawLine(Point p1, Point p2, Color c);
	void DrawLine(Point p1, Point p2, float r, float g, float b);
	void DrawCircle(Point center, float radius, Color c);
	void DrawCircle(Point center, float radius, float r, float g, float b);
	void DrawRect(Rect rect, Color c);
	void DrawRect(Rect rect, Color c, bool filled);
	/* Draws Image in game coordinates */
	void DrawImage(float x, float y, Image* img);
	/* Draws Image in game coordinates */
	void DrawImage(Point p, Image* img);
	void SetPrimitiveEnable(bool enable);
//Internal data. You don't need call any of this methods or modify variable
public:
	Graphics(Game* game);
	void DrawTargetImage(float x, float y, Image* img);
private:
	unsigned char* LoadImageInternal(string filename, GLuint* textureID, int* width, int* height);
    void DrawTargetPixel(Point p, float r, float g, float b);
	Game* game;
	Launcher* launcher;
	GLint prev_texID;
	bool primitive_enable;
};
    
}
