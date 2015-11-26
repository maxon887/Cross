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

//				private interface
GLuint Graphics3D::LoadShaders(ShaderInfo* shaders){
	if(shaders == NULL) {
		throw string("NULL shader info");
	}

    GLuint program = glCreateProgram();

    ShaderInfo* entry = shaders;
    while(entry->type != GL_NONE) {
        GLuint shader = glCreateShader( entry->type );
        entry->shader = shader;

		int size = 0;
		char* raw = (char*)launcher->LoadFile(entry->filename, &size);
		char* source = new GLchar[size + 1];
		memcpy(source, raw, size);
		delete[] raw;
		source[size] = 0;	//NULL terminated string
		/*
        if ( source == NULL ) {
            for ( entry = shaders; entry->type != GL_NONE; ++entry ) {
                glDeleteShader( entry->shader );
                entry->shader = 0;
            }

            return 0;
        }*/

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
        for(entry = shaders; entry->type != GL_NONE; ++entry){
            glDeleteShader( entry->shader );
            entry->shader = 0;
        }
        
		throw string("Shader compilation failed:\n" + msg);
    }

    return program;
}

void Graphics3D::DrawTargetPoint(int x, int y, Color c){

}