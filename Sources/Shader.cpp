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
#include "Shader.h"
#include "Launcher.h"

using namespace cross;

Shader::Shader(string filename) :
	type(0),
	handle(0){
	//file loading part
	string extension = filename.substr(filename.find_last_of(".") + 1);
	if(extension == "vert"){
		type = GL_VERTEX_SHADER;
	} else if(extension == "frag"){
		type = GL_FRAGMENT_SHADER;
	} else{
		throw string("Unknown file extension");
	}
	int size = 0;
	char* raw = (char*)launcher->LoadFile(filename, &size);
	GLchar* source = new GLchar[size + 1]; // +1 for null terminated string
	memcpy(source, raw, size);
	delete[] raw;
	source[size] = 0;
	//shader compilling part
	handle = glCreateShader(type);
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
		string msg(log);
		delete[] log;
		launcher->LogIt("Shader compilation failed:\n" + msg);
		throw string("Shader compilation failed:\n" + msg);
	} else{
#ifdef DEBUG
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
}

Shader::~Shader(){
	glDeleteShader(handle);
}