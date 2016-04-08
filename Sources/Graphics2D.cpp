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
#include "Graphics2D.h"
#include "GraphicsGL.h"
#include "Shaders/SpriteShaders.h"
#include "Shaders/PrimitiveShaders.h"
#include "Shaders/MonochromeShaders.h"
#include "Launcher.h"
#include "Game.h"
#include "Utils/Debugger.h"
#include "Sprite.h"
#include "Font.h"
#include "Utils/Misc.h"
#include "Camera2D.h"
#include "File.h"

#include "SOIL/SOIL.h"
#include "FreeType/ft2build.h"

#include <math.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

using namespace cross;

Graphics2D::Graphics2D() :
	camera(nullptr)
{
	launcher->LogIt("Graphics2D::Graphics2D()");
	sprite_shaders = new SpriteShaders();
	primitive_shaders = new PrimitiveShaders();
	monochrome_shaders = new MonochromeShaders();
	this->default_font = new Font("Engine/Fonts/VeraMono.ttf", 50, Color::White);
	default_camera = new Camera2D();
}

Graphics2D::~Graphics2D(){
	launcher->LogIt("Graphics2D::~Graphics2D");
	delete default_font;
	delete default_camera;
	delete sprite_shaders;
	delete primitive_shaders;
	delete monochrome_shaders;
}

void Graphics2D::SetCamera(Camera2D* camera){
	this->camera = camera;
}

Camera2D* Graphics2D::GetCamera() {
	if(camera) {
		return camera;
	} else {
		return default_camera;
	}
}

Camera2D* Graphics2D::GetDefaultCamera(){
	return default_camera;
}

void Graphics2D::DrawPoint(Vector2D pos, Color color){
	gfxGL->UseShaders(primitive_shaders);
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	glUniformMatrix4fv(primitive_shaders->uMVP, 1, GL_FALSE, mvp.GetData());
	glVertexAttribPointer(primitive_shaders->aPosition, 2, GL_FLOAT, GL_FALSE, 0, pos.GetData());
	glUniform4fv(primitive_shaders->uColor, 1, color.GetData());
	glEnableVertexAttribArray(primitive_shaders->aPosition);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Graphics2D::DrawLine(Vector2D p1, Vector2D p2, Color color){
	gfxGL->UseShaders(primitive_shaders);
	float vertices[4] = { p1.x, p1.y, p2.x, p2.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	glUniformMatrix4fv(primitive_shaders->uMVP, 1, GL_FALSE, mvp.GetData());
	glVertexAttribPointer(primitive_shaders->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glUniform4fv(primitive_shaders->uColor, 1, color.GetData());
	glEnableVertexAttribArray(primitive_shaders->aPosition);
	glDrawArrays(GL_LINES, 0, 2);
}

void Graphics2D::DrawRect(Rect rect, Color color){
	DrawRect(rect, color, false);
}

void Graphics2D::DrawRect(Rect rect, Color color, bool filled){
	gfxGL->UseShaders(primitive_shaders);
	float vertices[5 * 2] = {	rect.x, rect.y,
								rect.x + rect.width, rect.y,
								rect.x + rect.width, rect.y + rect.height,
								rect.x, rect.y + rect.height,
								rect.x, rect.y };
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	glUniformMatrix4fv(primitive_shaders->uMVP, 1, GL_FALSE, mvp.GetData());
	glVertexAttribPointer(primitive_shaders->aPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glUniform4fv(primitive_shaders->uColor, 1, color.GetData());
	glEnableVertexAttribArray(primitive_shaders->aPosition);
	if(filled){
		static GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	}else{
		glDrawArrays(GL_LINE_STRIP, 0, 5);
	}
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color){
	DrawCircle(center, radius, color, false);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled){
	DrawCircle(center, radius, color, filled, 30);
}

void Graphics2D::DrawCircle(Vector2D center, float radius, Color color, bool filled, int accuracy){
	gfxGL->UseShaders(primitive_shaders);
	int vertexCount = accuracy;

	// Create a buffer for vertex data
	float* buffer = new float[vertexCount * 2]; 
	int idx = 0;

	// Center vertex for triangle fan
	buffer[idx++] = center.x;
	buffer[idx++] = center.y;

	// Outer vertices of the circle
	int outerVertexCount = vertexCount - 1;

	for(int i = 0; i < outerVertexCount; ++i){
		float percent = (i / (float)(outerVertexCount - 1));
		float rad = percent * 2 * PI;

		//Vertex position
		float outer_x = center.x + radius * cos(rad);
		float outer_y = center.y + radius * sin(rad);

		buffer[idx++] = outer_x;
		buffer[idx++] = outer_y;
	}
	Camera* cam = GetCamera();
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
	mvp = mvp.Transpose();
	glUniformMatrix4fv(primitive_shaders->uMVP, 1, GL_FALSE, mvp.GetData());
	glVertexAttribPointer(primitive_shaders->aPosition, 2, GL_FLOAT, GL_FALSE, 0, buffer);
	//delete buffer;
	glUniform4fv(primitive_shaders->uColor, 1, color.GetData());
	glEnableVertexAttribArray(primitive_shaders->aPosition);
	if(filled){
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	}else{
		glDrawArrays(GL_LINE_LOOP, 2, vertexCount - 2);
	}
	delete buffer;
}

void Graphics2D::DrawText(Vector2D pos, string textStr){
	DrawText(pos, textStr, this->default_font);
}

void Graphics2D::DrawText(Vector2D pos, const string &textStr, Font* font){
	const char* text = textStr.c_str();

	while(*text){
		Sprite* ch = font->GetChar(*text);
		float advance = font->GetCharAdvance(*text);
		ch->SetPosition(pos);
		DrawSprite(ch, font->GetColor(), true);
		pos.x += advance;
		text++;
	}
}

void Graphics2D::DrawSprite(Sprite* sprite){
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Vector2D pos, Sprite* sprite){
	sprite->SetPosition(pos);
	DrawSprite(sprite, Color::White, false);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, bool monochrome){
	DrawSprite(sprite, color, GetCamera(), monochrome);
}

void Graphics2D::DrawSprite(Sprite* sprite, Color color, Camera2D* cam, bool monochrome){
	if(monochrome){
		gfxGL->UseShaders(monochrome_shaders);
	}else{
		gfxGL->UseShaders(sprite_shaders);
	}

	SAFE(glEnable(GL_BLEND));
	SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	Matrix mvp = cam->GetProjectionMatrix() * cam->GetViewMatrix() * sprite->GetModelMatrix();
	mvp = mvp.Transpose();
	SAFE(glUniformMatrix4fv(sprite_shaders->uMVP, 1, GL_FALSE, mvp.GetData()));
	SAFE(glUniform4fv(sprite_shaders->uColor, 1, color.GetData()));

	SAFE(glBindTexture(GL_TEXTURE_2D, sprite->GetTextureID()));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	SAFE(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	SAFE(glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO));
	SAFE(glEnableVertexAttribArray(sprite_shaders->aPosition));
	SAFE(glVertexAttribPointer(sprite_shaders->aPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0));
	SAFE(glEnableVertexAttribArray(sprite_shaders->aTexCoord));
	SAFE(glVertexAttribPointer(sprite_shaders->aTexCoord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLfloat*)0 + 2));
	
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO));
	SAFE(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0));
	SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

Sprite* Graphics2D::CreateImage(Sprite* src, Rect area, float scaleFactor){
	Sprite* img = new Sprite(src->GetTextureID(), src->GetTextureWidth(), src->GetTextureHeight(), area);
	img->SetScale(scaleFactor);
	return img;
}

Sprite* Graphics2D::LoadImage(string filename, float scale){
	Sprite* sprite = LoadImage(filename);
	sprite->SetScale(scale);
	return sprite;
}

Sprite* Graphics2D::LoadImage(string filename){
	Debugger::Instance()->StartCheckTime();
	int width, height;
	File* imageFile = launcher->LoadFile(filename);
	CRByte* image = SOIL_load_image_from_memory(imageFile->data, imageFile->size, &width, &height, 0, SOIL_LOAD_RGBA);
	delete imageFile;

	if(image == NULL){
		throw CrossException("SOIL can't convert file:\n Pay attention on image color channels");
	}
	Sprite* sprite = LoadImage(image, 4, width, height);
	SOIL_free_image_data(image);
	string debugMsg = "" + filename + " loaded in ";
	Debugger::Instance()->StopCheckTime(debugMsg);
	return sprite;
}

Sprite* Graphics2D::LoadImage(CRByte* image, int bytesPerChannel, int width, int height){
	GLuint textureID;
	Rect region(0, 0, (float)width, (float)height);
	int newWidth = 1;
	int newHeight = 1;
	while(newWidth < width) {
		newWidth *= 2;
	}
	while(newHeight < height) {
		newHeight *= 2;
	}
	//Create power of two texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	CRByte* newImage = NULL;
	if(newWidth > width || newHeight > height){
		newImage = (CRByte*)malloc(bytesPerChannel * newWidth * newHeight);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * newWidth * bytesPerChannel, image + i * width * bytesPerChannel, width * bytesPerChannel);
			//Clamp to edge effect
			if(newWidth > width){
				memcpy(newImage + i * newWidth * bytesPerChannel + width * bytesPerChannel, image + i * width * bytesPerChannel, bytesPerChannel);
			}
		}
		//Clamp to edge effect
		if(newHeight > height){
			memcpy(newImage + height * bytesPerChannel * newWidth, image + (height - 1) * width * bytesPerChannel, width * bytesPerChannel);
		}
		width = newWidth;
		height = newHeight;
		image = newImage;
	}

	switch(bytesPerChannel) {
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		break;
	default:
		throw CrossException("Unknown bit depth");
	}
	if(newImage){
		free(newImage);
	}

	Sprite* img = new Sprite(textureID, width, height, region);
	glBindTexture(GL_TEXTURE_2D, 0);
	return img;
}

void Graphics2D::ReleaseSprite(Sprite* img){
	if (img == nullptr) {
		throw CrossException("Can't release NULL image");
	}
	GLuint texID = img->GetTextureID();
	glDeleteTextures(1, &texID);
	glDeleteBuffers(1, &img->VBO);
	delete img;
}

void Graphics2D::Update(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics2D::SetClearColor(Color color){
	glClearColor(color.R, color.G, color.B, 1.f);
}

Font* Graphics2D::GetDefaultFont(){
	return default_font;
}