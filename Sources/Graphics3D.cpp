#include "Graphics3D.h"
#include "Game.h"
#include "Launcher.h"

using namespace cross;

Graphics3D::Graphics3D():
		program(0){
	launcher->LogIt("Graphics3D::Graphics3D()");
	GLint magorV;
	GLint minorV;
	glGetIntegerv(GL_MAJOR_VERSION, &magorV);
	glGetIntegerv(GL_MINOR_VERSION, &minorV);
	launcher->LogIt("OpenGL " + to_string(magorV) + "." + to_string(minorV));
	GLint maxVertexAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	launcher->LogIt("Max Vertex Attributes: " + to_string(maxVertexAttribs));
#ifdef WIN
	if (glewInit()) {
		launcher->LogIt("Unable to initialize GLEW");
	}
#endif
}

void Graphics3D::AttachShader(Shader* shader){
	if(program == 0){
		program = glCreateProgram();
	}
	glAttachShader(program, shader->handle);
}

GLuint Graphics3D::CompileProgram(){
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		string msg(log);
		delete[] log;
		launcher->LogIt("Shader compilation failed:\n" + msg);
		throw string("Shader compilation failed:\n" + msg);
	}
	glUseProgram(program);
#ifdef DEBUG
	GLsizei len;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &len);
#endif
	return program;
}

void Graphics3D::DrawPoint(Point p, Color c){
	GLfloat vertex[6] = { p.x, p.y, c.R, c.G, c.B };
	GLuint posAttrib = glGetAttribLocation(program, "a_position");
	GLuint colorAttrib = glGetAttribLocation(program, "a_color");
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6, vertex);
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 6, vertex + 2);
	glDrawArrays(GL_POINTS, 0, 1);
	/* 
	GLfloat vertexPosition[2] = { p.x, p.y };
	GLfloat vertexColor[3] = { c.R, c.G, c.B };
	GLint posAttrib = glGetAttribLocation(program, "a_position");
	GLint colorAttrib = glGetAttribLocation(program, "a_color");
	glVertexAttrib3fv(colorAttrib, vertexColor);
	glVertexAttrib2fv(posAttrib, vertexPosition);
	glDrawArrays(GL_POINTS, 0, 1);*/
}