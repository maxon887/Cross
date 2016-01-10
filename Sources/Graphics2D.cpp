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
#include "VertexShader.h"
#include "Launcher.h"
#include "Game.h"
#include "Utils\Debuger.h"
#include "Image.h"

#include "SOIL/SOIL.h"

#define BYTES_PER_CHANNEL 4

using namespace cross;

Graphics2D::Graphics2D(){
	launcher->LogIt("Graphics2D::Graphics2D()");
	vertex_shader = new VertexShader("Engine/image.vert");
	fragment_shader = new Shader("Engine/image.frag");
	AttachShader(vertex_shader);
	AttachShader(fragment_shader);
	CompileProgram();
	vertex_shader->Initialize(program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection = Matrix::CreateOrthogonalProjection(-1, game->GetWidth(), -1, game->GetHeight(), 1, -1);
	glUniformMatrix4fv(vertex_shader->uProjectionLoc, 1, GL_TRUE, projection.GetData());
}

Graphics2D::~Graphics2D(){
	delete vertex_shader;
	delete fragment_shader;
}

void Graphics2D::Clear(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics2D::DrawImage(Vector2D pos, Image* img){
	img->SetPosition(pos);
	glBindTexture(GL_TEXTURE_2D, img->GetTextureID());
	glVertexAttribPointer(vertex_shader->aPositionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices());
	glVertexAttribPointer(vertex_shader->aTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), img->GetVertices() + 2);
	glEnableVertexAttribArray(vertex_shader->aPositionLoc);
	glEnableVertexAttribArray(vertex_shader->aTexCoordLoc);
	glUniformMatrix4fv(vertex_shader->uModelLoc, 1, GL_TRUE, img->GetModel());

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
		unsigned char* newImage = (unsigned char*)malloc(BYTES_PER_CHANNEL * new_width * new_height);
		for(int i = 0; i < height; i++){
			memcpy(newImage + i * new_width * BYTES_PER_CHANNEL, image + i * width * BYTES_PER_CHANNEL, width * BYTES_PER_CHANNEL);
			//Clamp to edge effect
			if(new_width > width){
				memcpy(newImage + i * new_width * BYTES_PER_CHANNEL + width * BYTES_PER_CHANNEL, image + i * width * BYTES_PER_CHANNEL, BYTES_PER_CHANNEL);
			}
		}
		//Clamp to edge effect
		if(new_height > height){
			memcpy(newImage + height * BYTES_PER_CHANNEL * new_width, image + (height - 1) * width * BYTES_PER_CHANNEL, width * BYTES_PER_CHANNEL);
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