#pragma once
#include "Cross.h"
#include "Base/String.h"

#ifdef WIN
#ifdef GLES
#include "Platform/Windows/GLES2/gl2.h"
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
#elif MACOS || LINUX
#include <GL/glew.h>
#endif


#if defined(CROSS_DEBUG)
#define SAFE(X) \
{\
	(X);	  \
	GraphicsGL::CheckGLError(__FILE__, __LINE__); \
}
#else
#define SAFE(X) X;
#endif

namespace cross{

/*	Class containing specific OpenGL features */
class GraphicsGL {
public:
	static void CheckGLError(const char* file, U32 line);
	static void ClearGLErrorBuffer();
	
	GraphicsGL();
	~GraphicsGL();

	const String& GetShaderVersion() const;

	bool IsIntelRenderer() const;
	
private:
	String shader_version = "100";
	bool is_intel_renderer = false;

	void WindowResizeHandle(S32 width, S32 height);
};

}
