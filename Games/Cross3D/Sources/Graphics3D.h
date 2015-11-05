#pragma once
#include "Color.h"
#include "Point.h"

#ifdef WIN
#include "GL\glew.h"
#include "GL\wglew.h".
#elif ANDROID
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

namespace cross{

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;


class Graphics3D{
public:
	Graphics3D();
	GLuint LoadShaders(ShaderInfo* shaders);
};

}
