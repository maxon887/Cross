#include "Graphics3D.h"
#include "Game.h"
#include "Launcher.h"

using namespace cross;

Graphics3D::Graphics3D(){
#ifdef WIN
	if (glewInit()) {
		launcher->LogIt("Unable to initialize GLEW");
		//exit(EXIT_FAILURE);
	}
#endif
}

GLuint Graphics3D::LoadShader(GLenum type, string filename){
	GLuint shader = glCreateShader(type);
	if(!shader) throw string("Can not create shader");
	int size = 0;
	char* raw = (char*)launcher->LoadFile(filename, &size);
	GLchar* source = new GLchar[size + 1];
	memcpy(source, raw, size);
	source[size] = 0;	//NULL terminated string
	delete[] raw;

	glShaderSource(shader, 1, &source, NULL);
	delete[] source;

	glCompileShader(shader);
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled){
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		char* infoLog = new char[infoLen];
		glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
		string msg = string(infoLog);
		delete[] infoLog;
		glDeleteShader(shader);
		launcher->LogIt("Shader compilation failed:\n" + msg);
		throw string("Shader compilation failed:\n" + msg);
	}
	return shader;
}

GLuint Graphics3D::CreateProgram(string vertexShaderPath, string fragmentShaderPath){
	GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderPath);
	GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if(!linked){
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		char* infoLog = new char[infoLen];
		glGetProgramInfoLog(program, infoLen, NULL, infoLog);
				string msg = string(infoLog);
		delete[] infoLog;
		glDeleteProgram(program);
		launcher->LogIt("Error linking program:\n" + msg);
		throw string("Error linking program:\n" + msg);
	}
	return program;
}

/*
GLuint Graphics3D::LoadShaders(ShaderInfo* shaders){
	if(shaders == NULL) {
		throw string("NULL shader info");
	}

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while(entry->Type != GL_NONE) {
        GLuint shader = glCreateShader( entry->Type );
        entry->Shader = shader;

		int size = 0;
		char* raw = (char*)launcher->LoadFile(entry->Filename, &size);
		char* source = new GLchar[size + 1];
		memcpy(source, raw, size);
		delete[] raw;
		source[size] = 0;	//NULL terminated string

        glShaderSource(shader, 1, (const GLchar**)&source, NULL);
        delete[] source;

		glCompileShader( shader );

        GLint compiled;
        glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
        if(!compiled) {
            GLsizei len;
            glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( shader, len, &len, log );
			string msg = string(log);
			delete[] log;
			launcher->LogIt("Shader compilation failed:\n" + msg);
			throw string("Shader compilation failed:\n" + msg);
        }

        glAttachShader( program, shader );
        
        ++entry;
    }
    
    glLinkProgram( program );

    GLint linked;
    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if(!linked) {
        GLsizei len;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

        GLchar* log = new GLchar[len+1];
        glGetProgramInfoLog( program, len, &len, log );
		string msg = string(log);
		launcher->LogIt("Shader compilation failed:\n" + msg);

        delete [] log;
        for(entry = shaders; entry->Type != GL_NONE; ++entry){
            glDeleteShader( entry->Shader );
            entry->Shader = 0;
        }
        
		throw string("Shader compilation failed:\n" + msg);
    }

    return program;
}*/

void Graphics3D::DrawTargetPoint(int x, int y, Color c){

}