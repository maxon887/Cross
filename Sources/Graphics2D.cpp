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
#include "Launcher.h"
#include "Game.h"
#include "Utils\Debuger.h"
#include "Sprite.h"
#include "Font.h"

#include "SOIL/SOIL.h"

using namespace cross;

Graphics2D::Graphics2D():
	clear_color(Color::Black)
{
	this->font = new Font(DEFAULT_FONT, 50, Color::White);
	launcher->LogIt("Graphics2D::Graphics2D()");
	sprite_shaders = new SpriteShaders();
}

Graphics2D::~Graphics2D(){
	delete sprite_shaders;
}

void Graphics2D::Clear(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics2D::Clear(Color color){
	if(color != clear_color){
		glClearColor(color.R, color.G, color.B, 1.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics2D::SetTextFont(Font* font)
{
	delete(this->font);
	this->font = font;
}

void Graphics2D::DrawText(Vector2D pos, string text)
{
	this->DrawText(pos, text, this->font);
}

void Graphics2D::DrawText(Vector2D pos, string textStr, Font* font){
	FT_GlyphSlot g = font->face->glyph;
	GLuint tex;
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	const char* text = textStr.c_str();
	while(*text){
		if(FT_Load_Char(font->face, *text, FT_LOAD_RENDER))
			continue;

		glTexImage2D(GL_TEXTURE_2D,	0, GL_RED, g->bitmap.width, g->bitmap.rows,	0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		Rect region(0, 0, (float)g->bitmap.width, (float)g->bitmap.rows);
		int bearingX = g->metrics.horiBearingX >> 6;
		int bearingY = g->metrics.horiBearingY >> 6;
		Vector2D pivot(-bearingX, g->bitmap.rows - bearingY);
		Sprite ch(tex, g->bitmap.width, g->bitmap.rows, region, pivot);

		DrawSprite(pos, &ch, font->GetColor(), true);
		pos.x += (g->advance.x >> 6);
		text++;
	}
	glDeleteTextures(1, &tex);
}

void Graphics2D::DrawSprite(Vector2D pos, Sprite* img){
	DrawSprite(pos, img, Color::White, false);
}

void Graphics2D::DrawSprite(Vector2D pos, Sprite* img, Color color, bool monochrome){
	gfxGL->UseProgram(sprite_shaders->program);

	//parameterization
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection = Matrix::CreateOrthogonalProjection(-1, game->GetWidth(), -1, game->GetHeight(), 1, -1);
	glUniformMatrix4fv(sprite_shaders->uProjectionLoc, 1, GL_TRUE, projection.GetData());
	glUniform1i(sprite_shaders->uMonochrome, (GLint)monochrome);
	glUniform4fv(sprite_shaders->uColor, 1, (float*)&(color));

	img->SetPosition(pos);
	glBindTexture(GL_TEXTURE_2D, img->GetTextureID());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glVertexAttribPointer(sprite_shaders->aPositionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices());
	glVertexAttribPointer(sprite_shaders->aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices() + 2);
	glEnableVertexAttribArray(sprite_shaders->aPositionLoc);
	glEnableVertexAttribArray(sprite_shaders->aTexCoordLoc);
	glUniformMatrix4fv(sprite_shaders->uModelLoc, 1, GL_TRUE, img->GetModel());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, img->GetIndices());
}

Sprite* Graphics2D::CreateImage(Sprite* src, Rect area, float scaleFactor){
	Sprite* img = new Sprite(src->GetTextureID(), src->GetTextureWidth(), src->GetTextureHeight(), area);
	img->Scale(scaleFactor);
	return img;
}

Sprite* Graphics2D::LoadImage(string filename){
	return LoadImage(filename, game->GetScaleFactor());
}

Sprite* Graphics2D::LoadImage(string filename, float scaleFactor){
	debuger->StartCheckTime();
	GLuint textureID;
	int width, height;
	File* imageFile = launcher->LoadFile(filename);
	byte* image = SOIL_load_image_from_memory(imageFile->data, imageFile->size, &width, &height, 0, SOIL_LOAD_RGBA);
	delete imageFile;

	if(image == NULL){
		throw CrossException("SOIL can't convert file: " + filename + "\nPay attention on image color channels");
	}
	Sprite* sprite = LoadImage(image, 4, width, height);
	SOIL_free_image_data(image);
	return sprite;
}

Sprite* Graphics2D::LoadImage(byte* image, int bytesPerChannel, int width, int height){
	debuger->StartCheckTime();
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
	byte* newImage = NULL;
	if(newWidth > width || newHeight > height){
		newImage = (byte*)malloc(bytesPerChannel * newWidth * newHeight);
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
	string debugMsg = "Load image ";
	glBindTexture(GL_TEXTURE_2D, 0);
	debuger->StopCheckTime(debugMsg);
	return img;
}

void Graphics2D::ReleaseImage(Sprite* img){
	if(img == NULL) {
		throw CrossException("Can't release NULL image");
	}
	GLuint texID = img->GetTextureID();
	glDeleteTextures(1, &texID);
	delete img;
}