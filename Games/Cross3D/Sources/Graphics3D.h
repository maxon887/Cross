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

using namespace std;

namespace cross{

class Shader{
public:
	Shader(string filename);
	~Shader();
    GLenum type;
    GLuint handle;
};

class Graphics3D{
public:
	Graphics3D();
	void AttachShader(Shader* shader);
	void CompileProgram();
	void DrawPoint(Point p, Color c);
private:
	GLuint program;
};

}
