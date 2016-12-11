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
#include "VertexBuffer.h"
#include "Launcher.h"
#include "Game.h"
#include "File.h"
#include "Shaders/Shader.h"
#include "Shaders/MultiLightShader.h"
#include "Config.h"

using namespace cross;

void GraphicsGL::CheckGLError(const char* file, U32 line) {
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

GraphicsGL::GraphicsGL():
	framebuffer(0),
	colorbuffer(0),
	depthbuffer(0),
	quadVBO(0),
	quadEBO(0),
	bufferWidth(0),
	bufferHeight(0),
	offscreen_shader(NULL),
	colorbuffer_texture(NULL),
	regenerade_framebuffer(true)
{
		launcher->LogIt("GraphicsGL::GraphicsGL()");

#if defined(OPENGL)
		GLint magorV;
		GLint minorV;
		glGetIntegerv(GL_MAJOR_VERSION, &magorV);
		glGetIntegerv(GL_MINOR_VERSION, &minorV);
		launcher->LogIt("\tUsed OpenGL %d.%d", magorV, minorV);
		if(glewInit()) {
			throw CrossException("Unable to initialize GLEW");
		}
#else
		launcher->LogIt("\tUsed OpenGL ES 2.0");
#endif
		GLint value;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &value);
		launcher->LogIt("\tMax Vetex Uniforms: %d", value);

		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
		launcher->LogIt("\tMax Fragment Uniforms: %d", value);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
		launcher->LogIt("\tMax Vertex Attributes: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
		launcher->LogIt("\tMax Texture Size: %d", value);

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
		launcher->LogIt("\tMax Texture Units: %d", value);

		game->WindowResized += MakeDelegate(this, &GraphicsGL::WindowResizeHandle);

		if(config->IsOffscreenRender()){
			offscreen_shader = GetShader(DefaultShader::TEXTURE);
			offscreen_shader->Compile();
		}

		SAFE(glCullFace(GL_FRONT));
}

GraphicsGL::~GraphicsGL(){
	SAFE(glDeleteBuffers(1, &quadVBO));
	SAFE(glDeleteBuffers(1, &quadEBO));
	if(config->IsOffscreenRender()){
		delete offscreen_shader;
		SAFE(glDeleteBuffers(1, &framebuffer));
		SAFE(glDeleteBuffers(1, &colorbuffer));
		SAFE(glDeleteBuffers(1, &depthbuffer));
	}
}

void GraphicsGL::Start(){
	if(config->IsOffscreenRender()){
		SAFE(glGenFramebuffers(1, &framebuffer));

		GeneradeFramebuffer();

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

void GraphicsGL::Stop(){
	if(colorbuffer_texture){
		delete colorbuffer_texture;
	}
}

void GraphicsGL::PreProcessFrame(){
	if(config->IsOffscreenRender()){
		if(regenerade_framebuffer){
			GeneradeFramebuffer();
		}
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		SAFE(glViewport(0, 0, launcher->GetTargetWidth() / 2, launcher->GetTargetHeight() / 2));
	}
}

void GraphicsGL::PostProcessFrame(){
	if(config->IsOffscreenRender()){
		//binding default frame buffer
		SAFE(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		SAFE(glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight()));
		//drawing color buffer
		SAFE(glBindBuffer(GL_ARRAY_BUFFER, quadVBO));
		SAFE(glActiveTexture(GL_TEXTURE0));
		SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));

		UseShader(offscreen_shader);
		if(offscreen_shader->uColor != -1){
			SAFE(glUniform4fv(offscreen_shader->uColor, 1, Color::White.GetData()));
		}else{
			throw CrossException("Textured shader doesn't have color uniform");
		}

		if(offscreen_shader->aPosition != -1){
			SAFE(glEnableVertexAttribArray(offscreen_shader->aPosition));
			SAFE(glVertexAttribPointer(offscreen_shader->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
		}else{
			throw CrossException("Textured shader doesn't have verteces position coordinates");
		}

		if(offscreen_shader->aTexCoords != -1){
			SAFE(glEnableVertexAttribArray(offscreen_shader->aTexCoords));
			SAFE(glVertexAttribPointer(offscreen_shader->aTexCoords, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));
		}else{
			throw CrossException("Textured shader doesn't have texure coordinates");
		}

		if(offscreen_shader->uMVP != -1){
			Matrix mvp = Matrix::Identity;
			SAFE(glUniformMatrix4fv(offscreen_shader->uMVP, 1, GL_FALSE, mvp.GetData()));
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
	Shader* shader = NULL;
	switch(type) {
	case DefaultShader::SIMPLE:
		shader = new Shader("Engine/Shaders/simple.vert", "Engine/Shaders/simple.frag");
		shader->AddProperty("Color", "uColor");
		break;
	case DefaultShader::MONOCHROME:
		shader = new Shader("Engine/Shaders/texture.vert", "Engine/Shaders/texture.frag");
		shader->AddMakro("MONOCHROME");
		shader->AddProperty("Texture", "uTexture");
		break;
	case DefaultShader::TEXTURE:
		shader = new Shader("Engine/Shaders/texture.vert", "Engine/Shaders/texture.frag");
		shader->AddProperty("Texture", "uTexture");
		break;
	case DefaultShader::MULTI_LIGHT:
		shader = new MultiLightShader();
		shader->AddProperty("Transparency", "uTransparency", 1.f);
		break;
	default:
		throw CrossException("Unknown shader type");
	}
	return shader;
}

Texture* GraphicsGL::GetColorBuffer(){
	if(config->IsOffscreenRender()){
		if(!colorbuffer_texture){
			colorbuffer_texture = new Texture(colorbuffer, bufferWidth, bufferHeight, 4, Texture::Filter::NEAREST);
		}
		return colorbuffer_texture;
	}else{
		throw CrossException("You can obtain colorbuffer only if postprocess using");
	}
}

void GraphicsGL::UseShader(Shader* shader){
	if(shader){
		if(shader->IsCompiled()){
			SAFE(glUseProgram(shader->GetProgram()));
		}else{
			throw CrossException("Shader not compiled");
		}
	}else{
		throw CrossException("Attempt to draw with NULL shader");
	}
}

void GraphicsGL::GeneradeFramebuffer(){
	SAFE(glDeleteBuffers(1, &framebuffer));
	SAFE(glDeleteBuffers(1, &colorbuffer));
	SAFE(glDeleteBuffers(1, &depthbuffer));
	if(colorbuffer_texture){
		delete colorbuffer_texture;
		colorbuffer_texture = NULL;
	}
	//generade color buffer
	SAFE(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
	//generate color buffer
	bufferWidth = launcher->GetTargetWidth() / 2;
	bufferHeight = launcher->GetTargetHeight() / 2;

	SAFE(glGenTextures(1, &colorbuffer));
	SAFE(glBindTexture(GL_TEXTURE_2D, colorbuffer));
	SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bufferWidth, bufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
	SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	SAFE(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorbuffer, 0));
	//generate depth buffer
	SAFE(glGenRenderbuffers(1, &depthbuffer));
	SAFE(glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer));
	SAFE(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, bufferWidth, bufferHeight));
	SAFE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer));
	regenerade_framebuffer = false;

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		throw CrossException("Can not initialize second frame buffer");
	}
}

void GraphicsGL::WindowResizeHandle(S32 width, S32 height){
	SAFE(glViewport(0, 0, width, height));
	if(config->IsOffscreenRender()){
		//GeneradeFramebuffer();
		regenerade_framebuffer = true;
	}
}
