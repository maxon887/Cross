#include "Graphics3D.h"
#include "Game.h"
#include "Launcher.h"

using namespace cross;

Shader::Shader(string filename):
		type(0),
		handle(0){
	//file loading part
	string extension = filename.substr(filename.find_last_of(".") + 1);
	if(extension == "vert"){
		type = GL_VERTEX_SHADER;
	}else if(extension == "frag"){
		type = GL_FRAGMENT_SHADER;
	}else{
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
	}
}

Shader::~Shader(){
	glDeleteShader(handle);
}

Graphics3D::Graphics3D():
		program(0){
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

void Graphics3D::CompileProgram(){
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
}
