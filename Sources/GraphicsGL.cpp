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
#include "VertexBuffer.h"
#include "Launcher.h"
#include "Game.h"
#include "File.h"
#include "Shaders/Shader.h"
#include "Shaders/MultiLightShader.h"

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, unsigned int line) {
	GLenum err;
	err = glGetError();
	while(err != GL_NO_ERROR) {
		char* error = new char[255];
		switch(err) {
		case GL_INVALID_OPERATION:    
			strcpy(error, "INVALID_OPERATION");      
			break;
		case GL_INVALID_ENUM:     
			strcpy(error, "INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:     
			strcpy(error, "INVALID_VALUE");         
			break;
		case GL_OUT_OF_MEMORY:     
			strcpy(error, "OUT_OF_MEMORY");          
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  
			strcpy(error, "INVALID_FRAMEBUFFER_OPERATION");  
			break;
		default: strcpy(error, "Unknown error");  
			break;
		}
		launcher->LogIt("[ERROR] Rendering error number: %s in %s : %d", error, file, line);
		//throw CrossException("OpenGL error: %s in %s : %d", error, file, line);
		delete[] error;
		err = glGetError();
}
}

GraphicsGL::GraphicsGL() :
	shaders(DefaultShader::NONE),	//create place holders for NONE shaders
	off_screen_rendering(false)
{
		launcher->LogIt("GraphicsGL::GraphicsGL()");
#if defined (EDITOR) || defined(OPENGL)
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		launcher->LogIt("OpenGL " + to_string(magorV) + "." + to_string(minorV));
		if(glewInit()) {
			throw CrossException("Unable to initialize GLEW");
		}
#endif
		GLint value;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value);
		launcher->LogIt("Max Vetex Uniforms: %d", value);

		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
		launcher->LogIt("Max Fragment Uniforms: %d", value);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
		launcher->LogIt("Max Vertex Attributes: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		launcher->LogIt("Max Texture Size: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		launcher->LogIt("Max Texture Units: %d", value);

		game->WindowResized += MakeDelegate(this, &GraphicsGL::WindowResizeHandle);
}

GraphicsGL::~GraphicsGL(){
	for(Shader* shader : shaders){
		delete shader;
	}
	SAFE(glDeleteBuffers(1, &quadVBO));
	SAFE(glDeleteBuffers(1, &quadEBO));
}

void GraphicsGL::Start(){
	if(off_screen_rendering){
			SAFE(glGenFramebuffers(1, &framebuffer));
			//generate color buffer
			SAFE(glGenTextures(1, &colorbuffer));
			SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			//generate depth buffer
			SAFE(glGenTextures(1, &depthbuffer));
			SAFE(glBindTexture(GL_TEXTURE_2D, depthbuffer));
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL));

			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

			SAFE(glBindTexture( GL_TEXTURE_2D, 0));
			//setup frame buffer
			SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
			SAFE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0));
			SAFE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthbuffer, 0));

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
				throw CrossException("Can not initialize second frame buffer");
			}

			SAFE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			//quad setub
			static const float quadVertices[] = {
				-1.0f, -1.0f,		0.f, 0.f,
				 1.0f, -1.0f,		1.f, 0.f,
				 1.0f,  1.0f,		1.f, 1.f,
				-1.0f,  1.0f,		0.f, 1.f,
			};
			SAFE(glGenBuffers(1, &quadVBO));
			SAFE(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
			SAFE(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));
			SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));

			static const GLushort quadIndices[] = { 0, 1, 2, 0, 2, 3 };
			SAFE(glGenBuffers(1, &quadEBO));
			SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO));
			SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW));
			SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		
		}
}

void GraphicsGL::PreProcessFrame(){
	if(off_screen_rendering){
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		SAFE(glViewport(0, 0, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f));
	}
}

void GraphicsGL::PostProcessFrame(){
	if(off_screen_rendering){
		//binding default frame buffer
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		SAFE(glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight()));
		//drawing color buffer
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
		SAFE(glActiveTexture(GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));

		Shader* shader = GetShader(DefaultShader::TEXTURE);
		UseShader(shader);
		if(shader->uColor != -1){
			SAFE(glUniform3fv(shader->uColor, 1, Color(Color::White).GetData()));
		}else{
			throw CrossException("Textured shader doesn't have color uniform");
		}

		if(shader->aPosition != -1){
			SAFE(glEnableVertexAttribArray(shader->aPosition));
			SAFE(glVertexAttribPointer(shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
		}else{
			throw CrossException("Textured shader doesn't have verteces position coordinates");
		}

		if(shader->aTexCoords != -1){
			SAFE(glEnableVertexAttribArray(shader->aTexCoords));
			SAFE(glVertexAttribPointer(shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));
		}else{
			throw CrossException("Textured shader doesn't have texure coordinates");
		}

		if(shader->uMVP != -1){
			Matrix mvp = Matrix::CreateIdentity();
			SAFE(glUniformMatrix4fv(shader->uMVP, 1, GL_FALSE, mvp.GetData()));
		}else{
			throw CrossException("Textured shader doesn't have MVP matrix");
		}

		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO));
		SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
		SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}

Shader* GraphicsGL::GetShader(DefaultShader type){
	Shader* shader = nullptr;
	if(shaders[type] == nullptr){
		switch(type) {
		case DefaultShader::SIMPLE:
			shader = new Shader("Engine/Shaders/simple.vert", "Engine/Shaders/simple.frag");
			shader->AddProperty(new Shader::Property("Color", Shader::Property::Type::VEC3, "uColor"));
			break;
		case DefaultShader::MONOCHROME:
			shader = new Shader("Engine/Shaders/monochrome.vert", "Engine/Shaders/monochrome.frag");
			shader->AddProperty(new Shader::Property("Texture", Shader::Property::Type::SAMPLER, "uTexture"));
			break;
		case DefaultShader::TEXTURE:
			shader = new Shader("Engine/Shaders/texture.vert", "Engine/Shaders/texture.frag");
			shader->AddProperty(new Shader::Property("Texture", Shader::Property::Type::SAMPLER, "uTexture"));
			break;
		case DefaultShader::MULTI_LIGHT:
			shader = new MultiLightShader();
			shader->AddProperty("Diffuse Texture", Shader::Property::SAMPLER, "uDiffuseTexture");
			shader->AddProperty("Specular Map", Shader::Property::SAMPLER, "uSpecularMap");
			shader->AddProperty("Shininess", Shader::Property::FLOAT, "uShininess");
			break;
		default:
			throw CrossException("Unknown shader type");
		}
		shader->Compile();
		shaders[type] = shader;
	}
	return shaders[type];
}

void GraphicsGL::UseShader(Shader* shader){
	if(shader){
		if(!shader->IsCompiled()){
			shader->Compile();
		}
		SAFE(glUseProgram(shader->GetProgram()));
	}else{
		throw CrossException("Attempt to draw with NULL shader");
	}
}

void GraphicsGL::WindowResizeHandle(int width, int height){
	SAFE(glViewport(0, 0, width, height));
}