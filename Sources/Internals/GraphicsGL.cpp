#include "GraphicsGL.h"
#include "System.h"

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, U32 line) {
	GLenum err = glGetError();
	while(err != GL_NO_ERROR) {
		char* error = CREATE char[255];
		switch(err) {
		case GL_INVALID_OPERATION:	
			strcpy(error, "INVALID_OPERATION");	  
			break;
		case GL_INVALID_ENUM:	 
			strcpy(error, "INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:	 
			strcpy(error, "INVALID_VALUE");		 
			break;
		case GL_OUT_OF_MEMORY:	 
			strcpy(error, "OUT_OF_MEMORY");		  
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  
			strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");  
			break;
		default: 
			strcpy(error, "Unknown error");  
			break;
		}
		CROSS_ASSERT(false, "Rendering error number: # in # : #", error, file, line);
		delete[] error;
		err = glGetError();
	}
}

void GraphicsGL::ClearGLErrorBuffer() {
	GLenum err = glGetError();
	while(err != GL_NO_ERROR) {
		err = glGetError();
	}
}

GraphicsGL::GraphicsGL() {
	os->LogIt("GraphicsGL::GraphicsGL()");

#ifdef OPENGL
	GLenum glewResult = glewInit();
#ifndef LINUX //somehow there are bug for linux where glewInit throw an error without any further problems
	if (glewResult != GLEW_OK)	{
		String error = (char*)glewGetErrorString(glewResult);
		CROSS_ASSERT(false, "Unable to initialize GLEW: #", error);
	}
#endif
#endif

	String renderer = (const char*)glGetString(GL_RENDERER);
	if(renderer.Find("Intel") != -1 || renderer.Find("intel") != -1) {
		is_intel_renderer = true;
	}
	os->LogIt("\tRenderer - #", renderer);
	os->LogIt("\tOpenGL version - #", (const char*)glGetString(GL_VERSION));
	shader_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	os->LogIt("\tGLSL version - " + shader_version);

	S32 first = shader_version.FindFirstOf("0123456789.");
	if(first != -1) {
		S32 last = shader_version.FindNonFirstOf("0123456789.", first);
		if(last != -1) {
			shader_version.Cut(first, last);
		} else {
			shader_version.Cut(first, shader_version.Length());
		}
	} else {
		CROSS_ASSERT(false, "Can not obtain shader version");
	}
	shader_version.Remove(".");
	
	GLint value;
	SAFE(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value));
	os->LogIt("\tMax Vertex Attributes: #", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value));
	os->LogIt("\tMax Texture Size: #", value);

	SAFE(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value));
	os->LogIt("\tMax Texture Units: #", value);

	os->LogIt("\tDevice DPI - #", os->GetScreenDPI());

	os->WindowResized.Connect(this, &GraphicsGL::WindowResizeHandle);

	SAFE(glFrontFace(GL_CW));
	SAFE(glCullFace(GL_FRONT));
}

GraphicsGL::~GraphicsGL() {
	os->WindowResized.Disconnect(this, &GraphicsGL::WindowResizeHandle);
}

const String& GraphicsGL::GetShaderVersion() const {
	return shader_version;
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height) {
	SAFE(glViewport(0, 0, width, height));
}

bool GraphicsGL::IsIntelRenderer() const {
	return is_intel_renderer;
}