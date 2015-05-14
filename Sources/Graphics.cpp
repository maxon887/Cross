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
	
#include "Graphics.h"
#include "Image.h"
#include "Debuger.h"
#include "Game.h"

Graphics::Graphics(Game* game){
	this->game = game;
	launcher = game->launcher;
	glViewport(0, 0, launcher->GetTargetWidth(), launcher->GetTargetHeight());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, launcher->GetTargetWidth(), launcher->GetTargetHeight(), 0, 1, -1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	prev_texID = -1;
	launcher->LogIt("Graphics initialized");
}

void Graphics::Clear(ColorX c){
	Clear(c.R, c.G, c.B);
}

void Graphics::Clear(float r, float g, float b){
	glClearColor(r, g, b, 1);
	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::ScaleImage(Image* img, float factor){
	img->Scale(factor);
}

void Graphics::Rotate(Image* img, float angle){
	img->angle = angle;
}

Image* Graphics::CreateImage(Image* src, RectX reg){
	return CreateImage(src, reg, game->GetScaleFactor());
} 

Image* Graphics::CreateImage(Image* src, RectX region, float scaleFactor){
	if(src == NULL)
		throw "Attempt to create image from NULL";

	Image* img = new Image(src->GetTextureID(), (int)src->texWidth, (int)src->texHeight, region);
	img->Scale(scaleFactor);
	return img;
}

Image* Graphics::LoadImage(string filename){
	return LoadImage(filename, game->GetScaleFactor());
}

Image* Graphics::LoadImage(string filename, float scaleFactor){
	Debuger::StartCheckTime();
	unsigned int textureID;
	int width;
	int height;

	unsigned char* image = LoadImageInternal(filename, &textureID, &width, &height);

	int new_width = 1;
	int new_height = 1;
	while( new_width < width ) {
		new_width *= 2;
	}
	while( new_height < height ) {
		new_height *= 2;
	}
	//Create power of two texture
	if(new_width > width || new_height > height)
	{
		unsigned char* newImage = (unsigned char*)malloc(BYTES_PER_CHANNEL * new_width * new_height);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * new_width * BYTES_PER_CHANNEL, image + i * width * BYTES_PER_CHANNEL, width * BYTES_PER_CHANNEL );
			//Clamp to edge effect
            if(new_width > width){
                memcpy(newImage + i * new_width * BYTES_PER_CHANNEL + width * BYTES_PER_CHANNEL, image + (i + 1) * width * BYTES_PER_CHANNEL, BYTES_PER_CHANNEL);
            }
		}
		//Clamp to edge effect
		if(new_height > height){
			memcpy(newImage + height * BYTES_PER_CHANNEL * new_width, image + (height - 1) * width * BYTES_PER_CHANNEL, width * BYTES_PER_CHANNEL);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_width, new_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newImage);
		free(newImage);
	}else{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	SOIL_free_image_data(image);
	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	RectX region(0, 0, (float)width, (float)height);
	Image* img = new Image(textureID, new_width, new_height, region);
	img->Scale(scaleFactor);

    string debugMsg = "Load image " + filename + ": ";
	Debuger::StopCheckTime(debugMsg);
	return img;
}

Image* Graphics::LoadRepeatedImage(string filename, float w, float h){
	return LoadRepeatedImage(filename, w, h, game->GetScaleFactor());
}

Image* Graphics::LoadRepeatedImage(string filename, float w, float h, float scaleFactor){
	Debuger::StartCheckTime();
	unsigned int textureID;
	int width;
	int height;

	unsigned char* image = LoadImageInternal(filename, &textureID, &width, &height);

	//Create power of two texture
	textureID = SOIL_create_OGL_texture(image, width, height, 4, 0, SOIL_FLAG_POWER_OF_TWO);

	SOIL_free_image_data(image);

	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	RectX region(0, 0, (float)w, (float)h);
	Image* img = new Image(textureID, width, height, region);
	img->Scale(scaleFactor);

	string debugMsg = "Load image " + filename;
	Debuger::StopCheckTime(debugMsg);
	return img;
}

void Graphics::ReleaseImage(Image* img){
	const GLuint texID = (const GLuint)img->GetTextureID();
	glDeleteTextures(1, &texID);
	delete img;
}

unsigned char* Graphics::LoadImageInternal(string filename, GLuint* textureID, int* width, int* height){
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
#ifdef ANDROID
	LauncherAndroid * android = (LauncherAndroid*)launcher;
	unsigned char* file;
	int length;
	android->LoadFile(filename, &file, &length);
	unsigned char* image = SOIL_load_image_from_memory(file, length, width, height, 0, SOIL_LOAD_RGBA);
	free(file);
#else
	Launcher* launcher = game->launcher;
	string path = launcher->AssetsPath() + filename;
	unsigned char* image = SOIL_load_image(path.c_str(), width, height, 0, SOIL_LOAD_RGBA);
#endif
	if(image == NULL){
		string msg = "Can't load file: " + filename;
		throw msg;
	}
	return image;
}

void Graphics::DrawPixel(PointX p, ColorX c){
	DrawPixel(p, c.R, c.G, c.B);
}

void Graphics::DrawPixel(PointX p, float r, float g, float b){
	p.x *= game->GetScaleFactor();
	p.y *= game->GetScaleFactor();
	float vertices[] = { p.x, p.y };
	float colors[] = { r, g, b, 1 };
	static const unsigned short indices[] = { 0 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, colors);

	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_COLOR_ARRAY);
}

void Graphics::DrawLine(PointX p1, PointX p2, ColorX c){
	DrawLine(p1, p2, c.R, c.G, c.B);
}

void Graphics::DrawLine(PointX p1, PointX p2, float r, float g, float b){
	p1.x *= game->GetScaleFactor();
	p1.y *= game->GetScaleFactor();
	p2.x *= game->GetScaleFactor();
	p2.y *= game->GetScaleFactor();
	float vertices[] = { p1.x, p1.y, p2.x, p2.y };
	float colors[] = { r, g, b, 1, r, g, b, 1 };
	static const unsigned short indices[] = { 0, 1 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, colors);

	glDrawElements(GL_LINES, 2, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_COLOR_ARRAY);
}

void Graphics::DrawCircle(PointX center, float radius, ColorX c){
	DrawCircle(center, radius, c.R, c.G, c.B);
}

void Graphics::DrawCircle(PointX c, float radius, float r, float g, float b){
	int sqrRad = radius * radius;
	for ( int x = -radius * 0.7071068; x <= (radius * 0.7071068 + .5); x++ )
	{
		int y = sqrt( double( sqrRad ) - double ( x*x )) + .5;
		DrawPixel(PointX(x + c.x,y + c.y),r,g,b);
		DrawPixel(PointX(x + c.x,-y + c.y), r,g,b);
		DrawPixel(PointX(c.x + y, c.y + x), r,g,b);
		DrawPixel(PointX(c.x - y, c.y + x), r,g,b);
	}
}

void Graphics::DrawRect(RectX rect, ColorX c){
	DrawRect(rect, c.R, c.G, c.B);
}

void Graphics::DrawRect(RectX rect, float r, float g, float b){
	DrawLine(PointX(rect.x, rect.y), PointX(rect.x, rect.y + rect.height), r, g, b);
	DrawLine(PointX(rect.x, rect.y), PointX(rect.x + rect.width, rect.y), r, g, b);
	DrawLine(PointX(rect.x, rect.y + rect.height), PointX(rect.x + rect.width, rect.y + rect.height), r, g, b);
	DrawLine(PointX(rect.x + rect.width, rect.y), PointX(rect.x + rect.width, rect.y + rect.height), r, g, b);
}

void Graphics::DrawImage(float x, float y, Image* img){
	x = x * game->GetScaleFactor();
	y = y * game->GetScaleFactor();
	DrawTargetImage(x + .5f, y + .5f, img);
}

void Graphics::DrawImage(PointX p, Image* img){
	p.x = p.x * game->GetScaleFactor();
	p.y = p.y * game->GetScaleFactor();
	DrawTargetImage(p.x, p.y, img);
}

void Graphics::DrawTargetImage(float x, float y, Image* img){
	if(img == NULL)
		throw "Attempt to draw NULL image";
	if(prev_texID != img->GetTextureID()){
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, img->GetTextureID());
		prev_texID = img->GetTextureID();
		//glDisable(GL_TEXTURE_2D);
	}
	
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0);
	
	glRotatef(-img->angle, 0.0f, 0.0f, 1.0f);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	float* verts = img->GetVertices(); 
	glVertexPointer(2, GL_FLOAT, 16, verts);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	verts += 2;
	glTexCoordPointer(2, GL_FLOAT, 16, verts);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, Image::indices);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(-x, -y, 0);
}
