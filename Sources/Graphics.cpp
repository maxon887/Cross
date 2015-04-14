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
}

void Graphics::Clear(float r, float g, float b){
	glClearColor(r, g, b, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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

	Image* img = new Image(src->GetTextureID(), (int)src->GetWidth(), (int)src->GetHeight(), region);
	img->Scale(scaleFactor);
	return img;
}

Image* Graphics::LoadImage(const char* filename){
	return LoadImage(filename, game->GetScaleFactor());
}

Image* Graphics::LoadImage(const char* filename, float scaleFactor){
	Launcher* launcher = game->launcher;
    sprintf(str_buffer, "%s/%s", launcher->DataPath(), filename);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width;
	int height;

	unsigned char* image = SOIL_load_image(str_buffer, &width, &height, 0, SOIL_LOAD_RGBA);
#ifdef ANDROID
	LauncherAndroid * android = (LauncherAndroid*)launcher;
	android->FileFromAssets(filename);
	image = SOIL_load_image(str_buffer, &width, &height, 0, SOIL_LOAD_RGBA);
#endif
	if(image == NULL){
		sprintf(str_buffer, "Can't load file: %s", filename);
		throw str_buffer;
	}
	textureID = SOIL_create_OGL_texture(image, width, height, 4, 0, SOIL_FLAG_POWER_OF_TWO);
	SOIL_free_image_data(image);

	//parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	RectX region(0, 0, (float)width, (float)height);
	Image* img = new Image(textureID, width, height, region);
	img->Scale(scaleFactor);
	return img;
}

void Graphics::DrawImage(float x, float y, Image* img){
	x = x * game->GetScaleFactor();
	y = y * game->GetScaleFactor();
	DrawTargetImage(x + .5, y + .5, img);
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
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, img->GetTextureID());
		prev_texID = img->GetTextureID();
	}

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

}
