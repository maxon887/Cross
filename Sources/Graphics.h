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
#elif __APPLE__
    #include <OpenGLES/ES1/gl.h>
#elif ANDROID
    #include <GLES/gl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string>	
#include "SOIL.h"
#include "Game.h"
#include "Launcher.h"
//#include "Image.h"
#include "PointX.h"

class Image;

class Graphics{
public:

	Graphics(Game* game);
	/* Clear screen with particular color */
	void Clear(float r, float g, float b);
	Image* LoadImage(const char* filename);
	void Test();
	//void Init(Launcher* launcher);
	//void Test();
	void DrawTargetImage(PointX p, Image* img); //temporary
private:
	Game* game;

	char prev_tex_filename[128];;
	int prev_tex_id;
	int prev_tex_width;
	int prev_tex_height;
};
