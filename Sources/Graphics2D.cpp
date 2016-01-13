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
#include "Exception.h"
#include "SpriteShaders.h"
#include "TexterShaders.h"
#include "Launcher.h"
#include "Game.h"
#include "Utils\Debuger.h"
#include "Image.h"
#include "TexterAdvanced.h"

#include "SOIL/SOIL.h"

#define BYTES_RGBA 4
#define BYTES_RGB  3

using namespace cross;

Graphics2D::Graphics2D(){
	launcher->LogIt("Graphics2D::Graphics2D()");
	sprite_shaders = new SpriteShaders();
	texter_shaders = new TexterShaders();

	texter = new TexterAdvanced();
	File* fontFile = launcher->LoadFile("Engine/times.ttf");
	texter->LoadFont(fontFile);
	/*
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(uniform_tex, 0);*/



	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

Graphics2D::~Graphics2D(){
	delete sprite_shaders;
}

void Graphics2D::Clear(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics2D::DrawText(Vector2D pos, string textStr){
	gfxGL->UseProgram(texter_shaders->program);
	FT_GlyphSlot g = texter->face->glyph;

	GLuint tex;
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	//glUniform1i(uniform_tex, 0);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnableVertexAttribArray(texter_shaders->aPosition);
	//parameterization
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	const char* text = textStr.c_str();
	const char* p = text;
	while(*p){
		if(FT_Load_Char(texter->face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		GLfloat red[4] = { 1, 0, 0, 1 };
		glUniform4fv(texter_shaders->uColor, 1, red);

		//FT_Set_Pixel_Sizes(texter->face, 0, 48);

		float sx = 2.0 / launcher->GetTargetWidth();
		float sy = 2.0 / launcher->GetTargetHeight();

		//float x2 = pos.x + g->bitmap_left * sx;
		//float y2 = -pos.y - g->bitmap_top * sy;
		float x2 = g->bitmap_left * sx;
		float y2 = g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 },
		};

		//glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(texter_shaders->aPosition, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), box);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		pos.x += (g->advance.x >> 6) * sx;
		pos.y += (g->advance.y >> 6) * sy;

		p++;
	}
}

void Graphics2D::DrawImage(Vector2D pos, Image* img){
	gfxGL->UseProgram(sprite_shaders->program);

	//parameterization
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection = Matrix::CreateOrthogonalProjection(-1, game->GetWidth(), -1, game->GetHeight(), 1, -1);
	glUniformMatrix4fv(sprite_shaders->uProjectionLoc, 1, GL_TRUE, projection.GetData());

	img->SetPosition(pos);
	glBindTexture(GL_TEXTURE_2D, img->GetTextureID());
	glVertexAttribPointer(sprite_shaders->aPositionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices());
	glVertexAttribPointer(sprite_shaders->aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices() + 2);
	glEnableVertexAttribArray(sprite_shaders->aPositionLoc);
	glEnableVertexAttribArray(sprite_shaders->aTexCoordLoc);
	glUniformMatrix4fv(sprite_shaders->uModelLoc, 1, GL_TRUE, img->GetModel());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, img->GetIndices());
}

Image* Graphics2D::CreateImage(Image* src, Rect area, float scaleFactor){
	Image* img = new Image(src->GetTextureID(), (int)src->texWidth, (int)src->texHeight, area);
	img->Scale(scaleFactor);
	return img;
}

Image* Graphics2D::LoadImage(string filename){
	return LoadImage(filename, game->GetScaleFactor());
}

Image* Graphics2D::LoadImage(string filename, float scaleFactor){
	debuger->StartCheckTime();
	GLuint textureID;
	int width, height;
	byte* image = LoadImageInternal(filename, &width, &height);

	int new_width = 1;
	int new_height = 1;
	while(new_width < width) {
		new_width *= 2;
	}
	while(new_height < height) {
		new_height *= 2;
	}
	//Create power of two texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(new_width > width || new_height > height){
		byte* newImage = (byte*)malloc(BYTES_RGBA * new_width * new_height);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * new_width * BYTES_RGBA, image + i * width * BYTES_RGBA, width * BYTES_RGBA);
			//Clamp to edge effect
			if(new_width > width){
				memcpy(newImage + i * new_width * BYTES_RGBA + width * BYTES_RGBA, image + i * width * BYTES_RGBA, BYTES_RGBA);
			}
		}
		//Clamp to edge effect
		if(new_height > height){
			memcpy(newImage + height * BYTES_RGBA * new_width, image + (height - 1) * width * BYTES_RGBA, width * BYTES_RGBA);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_width, new_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newImage);
		free(newImage);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	SOIL_free_image_data(image);

	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	Rect region(0, 0, (float)width, (float)height);
	Image* img = new Image(textureID, new_width, new_height, region);
	img->Scale(scaleFactor);
	string debugMsg = "Load image " + filename + ": ";
	glBindTexture(GL_TEXTURE_2D, 0);
	debuger->StopCheckTime(debugMsg);
	return img;
}

Image* Graphics2D::LoadImage(byte* image, int width, int height){
	debuger->StartCheckTime();
	GLuint textureID;
	int new_width = 1;
	int new_height = 1;
	while(new_width < width) {
		new_width *= 2;
	}
	while(new_height < height) {
		new_height *= 2;
	}
	//Create power of two texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if(new_width > width || new_height > height){
		byte* newImage = (byte*)malloc(BYTES_RGB * new_width * new_height);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * new_width * BYTES_RGB, image + i * width * BYTES_RGB, width * BYTES_RGB);
			//Clamp to edge effect
			if(new_width > width){
				memcpy(newImage + i * new_width * BYTES_RGB + width * BYTES_RGB, image + i * width * BYTES_RGB, BYTES_RGB);
			}
		}
		//Clamp to edge effect
		if(new_height > height){
			memcpy(newImage + height * BYTES_RGB * new_width, image + (height - 1) * width * BYTES_RGB, width * BYTES_RGB);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, new_width, new_height, 0, GL_RGB, GL_UNSIGNED_BYTE, newImage);
		free(newImage);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	delete image;
	//SOIL_free_image_data(image);

	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	Rect region(0, 0, (float)width, (float)height);
	Image* img = new Image(textureID, new_width, new_height, region);
	img->Scale(game->GetScaleFactor());
	string debugMsg = "Load image ";
	glBindTexture(GL_TEXTURE_2D, 0);
	debuger->StopCheckTime(debugMsg);
	return img;
}

void Graphics2D::ReleaseImage(Image* img){
	if(img == NULL){
		throw CrossException("Can't release NULL image");
	}
	GLuint texID = img->GetTextureID();
	glDeleteTextures(1, &texID);
	delete img;
}

byte* Graphics2D::LoadImageInternal(string filename, int* width, int* height){
	File* imageFile = launcher->LoadFile(filename);
	byte* image = SOIL_load_image_from_memory(imageFile->data, imageFile->size, width, height, 0, SOIL_LOAD_RGBA);
	delete imageFile;

	if(image == NULL){
		string msg = "Can't load file: " + filename;
		throw msg;
	}
	return image;
}