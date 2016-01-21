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
#include "GraphicsGL.h"
#include "Launcher.h"
#include "File.h"

using namespace cross;

GraphicsGL::GraphicsGL(){
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
			throw CrossException("Unable to initialize GLEW");
		}
#endif
}

GLuint GraphicsGL::ComplileShader(string filename){
		//file loading part
		string extension = filename.substr(filename.find_last_of(".") + 1);
		GLuint type;
		if(extension == "vert"){
			type = GL_VERTEX_SHADER;
		} else if(extension == "frag"){
			type = GL_FRAGMENT_SHADER;
		} else{
			throw CrossException("Can't compile shader.\nUnknown file extension.");
		}
		File* file = launcher->LoadFile(filename);
		GLchar* source = new GLchar[file->size + 1]; // +1 for null terminated string
		memcpy(source, file->data, file->size);
		source[file->size] = 0;
		delete file;
		//shader compilling part
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)&source, NULL);
		delete[] source;
		source = NULL;

		glCompileShader(handle);
		GLint compiled;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
		if(!compiled) {
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(handle, len, &len, log);
			throw CrossException("Shader compilation failed:\n%s", log);
		} else{
#ifdef CROSS_DEBUG
			GLsizei len;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
			if(len > 1){
				GLchar* log = new GLchar[len + 1];
				glGetShaderInfoLog(handle, len, &len, log);
				string msg(log);
				delete[] log;
				launcher->LogIt("Shader compilation:\n" + msg);
			}
#endif
		}
		return handle;
}

void GraphicsGL::DeleteShader(GLuint handle){
	glDeleteShader(handle);
}

GLuint GraphicsGL::CreateProgram(){
	return glCreateProgram();
}

void GraphicsGL::DeleteProgram(GLuint program){
	glDeleteProgram(program);
}



void GraphicsGL::AttachShader(GLuint program, GLuint shader){
	glAttachShader(program, shader);
}

void GraphicsGL::CompileProgram(GLuint program){
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked) {
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(program, len, &len, log);
		throw CrossException("Shader compilation failed:\n %s", log);
	}
}

void GraphicsGL::UseProgram(GLuint program){
	glUseProgram(program);
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