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

#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl


/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8



// Types

class ESContext
{
public:
	/// Window width
	GLint       width;

	/// Window height
	GLint       height;

	/// Window handle
	EGLNativeWindowType  hWnd;

	/// EGL display
	EGLDisplay  eglDisplay;

	/// EGL context
	EGLContext  eglContext;

	/// EGL surface
	EGLSurface  eglSurface;

	/// Callbacks
	void (ESCALLBACK *drawFunc) (ESContext *);
	void (ESCALLBACK *keyFunc) (ESContext *, unsigned char, bool);
	void (ESCALLBACK *updateFunc) (ESContext *, float deltaTime);
};



//  Public Functions

int GLES_GO();

/// \brief Initialize ES framework context.  This must be called before calling any other functions.
/// \param esContext Application context

void ESUTIL_API esInitContext(ESContext *esContext);

//
/// \brief Create a window with the specified parameters
/// \param esContext Application context
/// \param title Name for title bar of window
/// \param width Width in pixels of window to create
/// \param height Height in pixels of window to create
/// \param flags Bitfield for the window creation flags 
///         ES_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
///         ES_WINDOW_ALPHA   - specifies that the color buffer should have alpha
///         ES_WINDOW_DEPTH   - specifies that a depth buffer should be created
///         ES_WINDOW_STENCIL - specifies that a stencil buffer should be created
///         ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
/// \return GL_TRUE if window creation is succesful, GL_FALSE otherwise
GLboolean ESUTIL_API esCreateWindow(ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags);

//
/// \brief Start the main loop for the OpenGL ES application
/// \param esContext Application context
//
void ESUTIL_API esMainLoop(ESContext *esContext);

//
/// \brief Register a draw callback function to be used to render each frame
/// \param esContext Application context
/// \param drawFunc Draw callback function that will be used to render the scene
//
void ESUTIL_API esRegisterDrawFunc(ESContext *esContext, void (ESCALLBACK *drawFunc) (ESContext*));

//
/// \brief Register an update callback function to be used to update on each time step
/// \param esContext Application context
/// \param updateFunc Update callback function that will be used to render the scene
//
void ESUTIL_API esRegisterUpdateFunc(ESContext *esContext, void (ESCALLBACK *updateFunc) (ESContext*, float));

//
/// \brief Register an keyboard input processing callback function
/// \param esContext Application context
/// \param keyFunc Key callback function for application processing of keyboard input
//
void ESUTIL_API esRegisterKeyFunc(ESContext *esContext,
	void (ESCALLBACK *drawFunc) (ESContext*, unsigned char, bool));
//
/// \brief Log a message to the debug output for the platform
/// \param formatStr Format string for error log.  
//
void ESUTIL_API esLogMessage(const char *formatStr, ...);

//
///
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint ESUTIL_API esLoadShader(GLenum type, char * filename);

//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
///        Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint ESUTIL_API esLoadProgram(GLuint vertexShader, GLuint fragmentShader);


// Create Win32 instance and window
GLboolean WinCreate(ESContext *esContext, const char *title);



// Start main windows loop
void WinLoop(ESContext *esContext);