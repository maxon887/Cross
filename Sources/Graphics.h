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

#ifdef _WIN32
    #include <Windows.h>
    #include <gl/GL.h>
    #define glOrthof glOrtho
    #define GL_CLAMP_TO_EDGE GL_CLAMP
    #undef LoadImage
#elif defined(__APPLE__) || defined(__APPLE_CC__)
    #include <OpenGLES/ES1/gl.h>
#elif defined(ANDROID) || defined(__ANDROID__)
    #include <GLES/gl.h>
	#include "LauncherAndroid.h"
#endif

#include "SOIL.h"
#include "Game.h"
#include "Launcher.h"

#define BUF_LEN 256

class Image;

class Graphics{
//User module
public:
	/* Clear screen with particular color */
	void Clear(float r, float g, float b);
	/* Scale Image */
	void ScaleImage(Image* img, float factor);
	/* Rotate Image counterclockwise in angle degree */
	void Rotate(Image* img, float angle);
	/* Create Image from source. data do not copy. */
	Image* CreateImage(Image* src, RectX region);
	/* Create Image from source. data do not copy. */
	Image* CreateImage(Image* src, RectX region, float scaleFactor);
	/* Load Image from file */
	Image* LoadImage(const char* filename);
	/* Draws particular image in game coordinates */
	void DrawImage(float x, float y, Image* img);
	/* Draws particular image in game coordinates */
	void DrawImage(PointX p, Image* img);
//Framework module. You don't need call any of this methods or modify variable
public:
	Graphics(Game* game);
private:
	void DrawTargetImage(float x, float y, Image* img);
	Game* game;
	Launcher* launcher;
	GLint prev_texID;
	char str_buffer[BUF_LEN];
};
