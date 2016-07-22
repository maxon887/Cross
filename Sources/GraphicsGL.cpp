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
#include "Graphics2D.h"
#include "Camera2D.h"
#include "Sprite.h"
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
	off_screen_rendering(true)
{
		launcher->LogIt("GraphicsGL::GraphicsGL()");
#if defined (WIN) && defined(OPENGL)
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
}

void GraphicsGL::Start(){
	if(off_screen_rendering){
			SAFE(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &default_framebuffer));

			SAFE(glGenFramebuffers(1, &framebuffer));
			texture = gfx2D->CreateTexture(4, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f, Texture::Filter::LINEAR);
			SAFE(glGenRenderbuffers(1, &depthbuffer));

			SAFE(glBindTexture(GL_TEXTURE_2D, texture->GetID()));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0));

			SAFE(glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer));
			SAFE(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f));

			SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
			SAFE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetID(), 0));
			SAFE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer));
		}
}

void GraphicsGL::PreProcessFrame(){
	SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
	SAFE(glViewport(0, 0, launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f));
}

void GraphicsGL::PostProcessFrame(){
	SAFE(glBindFramebuffer(GL_FRAMEBUFFER, default_framebuffer));
	SAFE(glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight()));
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Sprite sprite(texture, Rect(Vector2D(0.f, 0.f), texture->GetWidth(), texture->GetHeight()));
	//sprite.SetScale(3.f);
	//sprite.SetPosition(Vector2D(launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f));
	Vector2D position(launcher->GetTargetWidth() / 2.f, launcher->GetTargetHeight() / 2.f);
	float scaleFactor = launcher->GetTargetWidth() / gfx2D->GetCamera()->GetViewWidth();
	position /= scaleFactor;
	sprite.SetPosition(position);
	sprite.SetScale(2.f / scaleFactor);
	sprite.SetRotateX(180.f);
	//sprite.SetRotateY(180.f);
	//sprite.SetRotate(180.f);
	gfx2D->DrawSprite(&sprite);


}

Shader* GraphicsGL::GetShader(DefaultShader type){
	if(shaders[type] == nullptr){
		switch(type) {
		case DefaultShader::SIMPLE:
			shaders[type] = new Shader("Engine/Shaders/simple.vert", "Engine/Shaders/simple.frag");
			break;
		case DefaultShader::MONOCHROME:
			shaders[type] = new Shader("Engine/Shaders/monochrome.vert", "Engine/Shaders/monochrome.frag");
			break;
		case DefaultShader::TEXTURE:
			shaders[type] = new Shader("Engine/Shaders/texture.vert", "Engine/Shaders/texture.frag");
			break;
		case DefaultShader::MULTI_LIGHT:
			shaders[type] = new MultiLightShader();
			break;
		default:
			throw CrossException("Unknown shader type");
		}
	}
	return shaders[type];
}

void GraphicsGL::UseShader(Shader* shader){
	if(shader){
		SAFE(glUseProgram(shader->GetProgram()));
	}else{
		throw CrossException("Attempt to draw with NULL shader");
	}
}

void GraphicsGL::WindowResizeHandle(int width, int height){
	SAFE(glViewport(0, 0, width, height));
}