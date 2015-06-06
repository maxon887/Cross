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

#include "Launcher.h"
#include "RectX.h"

#ifdef WIN
    #include <Windows.h>
    #include <gl/GL.h>
#elif IOS
    #include <OpenGLES/ES1/gl.h>
#elif ANDROID
    #include <GLES/gl.h>
#endif

/* Class that reflected images loaded in to memory */
class Image{
public:
	/* Return virtual image width */
    float GetWidth();
	/* Return virtual image height */
    float GetHeight();

//Internal data. You don't need call any of this methods or modify variable
public:
    float angle;
	int texWidth;
	int texHeight;
    
	Image(GLuint id, int texWidth, int texHeight, RectX region);
	void Scale(float factor);
	GLuint GetTextureID();
	float* GetVertices();
private:
	RectX region;
	GLuint textureID;
	float vertices[16];
	float u1, v1;
	float u2, v2;
};