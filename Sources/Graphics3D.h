#pragma once
#include "Color.h"
#include "Point.h"
#include <string>

#ifdef WIN
#include "GL\glew.h"
#include "GL\wglew.h"
#elif ANDROID
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#endif

#include "Shader.h"

using namespace std;

namespace cross{

class Shader;

class Graphics3D{
public:
	Graphics3D();
	void AttachShader(Shader* shader);
	GLuint CompileProgram();
	void DrawPoint(Point p, Color c);
private:
	GLuint program;
};

}
