#pragma once
#include "Color.h"
#include "Point.h"

#include <string>
using namespace std;

#ifdef WIN
#include "GL\glew.h"
#include "GL\wglew.h"
#elif ANDROID
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

namespace cross{
	/*
struct ShaderInfo {
    GLenum Type;
    string Filename;
    GLuint Shader;
};*/


class Graphics3D{
public:
	Graphics3D();
	GLuint LoadShader(GLenum type, string filename);
	GLuint CreateProgram(string vertexShader, string framentShader);
	//GLuint LoadShaders(ShaderInfo* shaders);
	void DrawTargetPoint(int x, int y, Color c);
};

}
