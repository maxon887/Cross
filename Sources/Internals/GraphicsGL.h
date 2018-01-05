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

#if defined(WIN) || defined(EDITOR)
#ifdef GLES
#include "Platform/Windows/GLES2/gl2.h"
#include "Platform/Windows/EGL/egl.h"
#else
#include "Platform/Windows/glew/glew.h"
#include "Platform/Windows/glew/wglew.h"
#endif
#elif ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#elif IOS
#import <OpenGLES/ES2/glext.h>
#endif

#if defined(CROSS_DEBUG)
#define SAFE(X) \
{\
	(X);      \
	GraphicsGL::CheckGLError(__FILE__, __LINE__); \
}
#else
#define SAFE(X) X;
#endif

namespace cross{

/*	Class containing specific OpengGL features */
class GraphicsGL {
public:
	static void CheckGLError(const char* file, U32 line);
	static void ClearGLErrorBuffer();

	GraphicsGL();

	U32 GetShaderVersion() const;

private:
	U32 shaders_version				= 100;

	void WindowResizeHandle(S32 width, S32 height);
};

}
