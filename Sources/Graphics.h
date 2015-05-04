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

#include "Game.h"
#include "SOIL.h"
#include "Launcher.h"

#ifdef WIN
    #include <Windows.h>
    #include <gl/GL.h>
    #define glOrthof glOrtho
    #define GL_CLAMP_TO_EDGE GL_CLAMP
    #undef LoadImage
#elif IOS
    #include <OpenGLES/ES1/gl.h>
#elif ANDROID
    #include <GLES/gl.h>
	#include "LauncherAndroid.h"
#endif

#define BYTES_PER_CHANNEL 4

class Image;

/* Class responsible for drawing objects into the sceen */
class Graphics{
public:
	/*	Clear screen with particular color.
		Input color range from 0 to 1.*/
	void Clear(float r, float g, float b);
	/* Scale Image */
	void ScaleImage(Image* img, float factor);
	/* Rotate Image counterclockwise in angle degree */
	void Rotate(Image* img, float angle);
	/* Create Image from source. Data do not copy. */
	Image* CreateImage(Image* src, RectX region);
	/* Create Image from source with sacale factor. Data do not copy. */
	Image* CreateImage(Image* src, RectX region, float scaleFactor);
	/* Load Image from file */
	Image* LoadImage(string filename);
	/* Load scaled image from file */
	Image* LoadImage(string filename, float scaleFactor);
	/* Delete all data related with Image */
	void ReleaseImage(Image* img);
	/* Load scaled image filled with images*/
	Image* LoadRepeatedImage(string filename, float scaleFactor, float width, float height);
	/* Draws Iimage in game coordinates */
	void DrawImage(float x, float y, Image* img);
	/* Draws Image in game coordinates */
	void DrawImage(PointX p, Image* img);
//Internal data. You don't need call any of this methods or modify variable
public:
	Graphics(Game* game);
private:
	unsigned char* LoadImageInternal(string filename, GLuint* textureID, int* width, int* height);
	void DrawTargetImage(float x, float y, Image* img);
	Game* game;
	Launcher* launcher;
	GLint prev_texID;
	//char str_buffer[BUF_LEN];
};
