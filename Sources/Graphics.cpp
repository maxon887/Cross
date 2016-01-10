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
#include "Graphics.h"
#include "Exception.h"
#include "Launcher.h"
#include "Shader.h"

using namespace cross;

Graphics::Graphics() :
	program(0){
		launcher->LogIt("GraphicsGL::GraphicsGL()");
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		launcher->LogIt("OpenGL " + to_string(magorV) + "." + to_string(minorV));
		GLint maxVertexAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
		launcher->LogIt("Max Vertex Attributes: " + to_string(maxVertexAttribs));
#ifdef WIN
		if(glewInit()) {
			launcher->LogIt("Unable to initialize GLEW");
		}
#endif
}

void Graphics::AttachShader(Shader* shader){
	if(program == 0){
		program = glCreateProgram();
	}
	glAttachShader(program, shader->handle);
}

GLuint Graphics::CompileProgram(){
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
		throw CrossException("Shader compilation failed:\n" + msg);
	}
	glUseProgram(program);
#ifdef DEBUG
	GLsizei len;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &len);
#endif
	return program;
}
/*
void GraphicsGL::DrawPoint(Vector2D p, Color c){
	GLfloat vertex[6] = { p.x, p.y, c.R, c.G, c.B };
	GLuint posAttrib = glGetAttribLocation(program, "a_position");
	GLuint colorAttrib = glGetAttribLocation(program, "a_color");
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6, vertex);
	glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 6, vertex + 2);
	glDrawArrays(GL_POINTS, 0, 1);
}*/