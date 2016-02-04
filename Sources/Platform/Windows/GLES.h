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
#ifdef GLES
#include "Cross.h"
#include "GraphicsGL.h"

// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

class ESContext
{
public:
	// Window handle
	EGLNativeWindowType  hWnd;

	// EGL display
	EGLDisplay  eglDisplay;

	// EGL context
	EGLContext  eglContext;

	// EGL surface
	EGLSurface  eglSurface;
};


int GLES_GO();

/*
	\brief Create a window with the specified parameters
	\param esContext Application context
	\param width Width in pixels of window to create
	\param height Height in pixels of window to create
	\param flags Bitfield for the window creation flags 
	         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
	         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
	         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
	         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
	         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
	 \return GL_TRUE if window creation is succesful, GL_FALSE otherwise*/
GLboolean esCreateWindow(ESContext *esContext, GLuint flags);



#endif